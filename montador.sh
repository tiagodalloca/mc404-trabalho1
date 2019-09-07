#!/bin/bash

AUXDIR="./aux"
BINDIR="./bin"
RESDIR="./resultados"
OUTDIR="./saidas"
SRCDIR="./src"
TESTDIR="./testes"
TESTESFILE="tests.results"

# Parametros
COMPILE=0
TEST=0
EXEC=0
CLEAN=0
ERROR=0
PART=0

abort() {
    echo "ERRO: $1"
    exit 1
}

usage(){
    abort "Uso: $0 [-c|--compilar] [-e|--executar] [-t|--testar] [-l|--limpar] parte1|parte2"
}

limpar() {
    echo "Limpando arquivos..."
    make clean
    rm -rf $OUTDIR/*
}

compile() {
    echo "Compilando arquivos para $PART"
    export SRCDIR
    export BINDIR
    export AUXDIR

    if [ "$PART" == 'parte1' ]; then
        make montador-p1.x || abort "Compilando parte 1"
    else
        make montador-p2.x || abort "Compilando parte 2"
    fi
}

executar() {
    echo "Executando testes para $PART"
    if [ "$PART" == 'parte1' ]; then
        CMD=$BINDIR/montador-p1.x
    else
        CMD=$BINDIR/montador-p2.x
    fi

    for i in $(ls $TESTDIR | sort); do
        echo "**************** EXECUTANDO ENTRADA $i ($PART) **************** "
        echo "  Saida em $OUTDIR/${i%.*}.$PART"
        $CMD $TESTDIR/$i >$OUTDIR/${i%.*}.$PART 2>&1
        echo ""
    done
}

testar() {
    echo "Realizando testes para $PART"

    if [ "$PART" == 'parte1' ]; then
        LS="$OUTDIR/*.parte1"
    else
        LS="$OUTDIR/*.parte2"
    fi

    rm -rf $TESTESFILE
    echo "Teste $PART; Erro" > $TESTESFILE.$PART

    for i in $(ls $LS | sort); do
        echo "**************** TESTANDO SAIDA $i ($PART) **************** "
        echo "  Resultados em $TESTESFILE"
        sdiff --ignore-blank-lines --ignore-case $i $RESDIR/$(basename ${i%.*}.$PART)
        RES=$?
        echo "$(basename ${i%.*}).$PART; $RES" >> $TESTESFILE.$PART

        if [ "$RES" -eq "0" ]; then
            echo "---------------- OK. PARABENS ---------------- "
        else
            echo "---------------- ERRO NA SAIDA $(basename ${i%.*}).$PART ---------------- "
        fi

        echo ""
    done
}

# Checa os argumentos
for arg in "$@"; do
    if [[ "$arg" =~ ^\-+.*$ ]]; then
        case $1 in
            -c|--compilar) COMPILE=1 ;;
            -e|--executar) EXEC=1 ;;
            -t|--testar) TEST=1 ;;
            -l|--limpar) CLEAN=1 ;;
            *) echo "Argumento invalido: $1"; ERROR=1 ;;
        esac
        shift
    else
        break;
    fi
done
# Any error in arguments?
if [ "$ERROR" -ne "0" ]; then usage; fi

if [ "$1" == "parte1" ]; then
    PART="parte1"
elif [ "$1" == "parte2" ]; then
    PART="parte2"
else
    abort "Parte invalida informada: '$1'. Por favor, opte por 'parte1' ou 'parte2'"
fi

echo "Parametros para: $PART"
echo -e "COMPILAR? $COMPILE \t EXECUTAR? $EXEC \t TESTAR? $TEST \t LIMPAR? $CLEAN"

if [ "$CLEAN" -eq "1" ]; then
    limpar
fi

if [ "$COMPILE" -eq "1" ]; then
    compile
fi

if [ "$EXEC" -eq "1" ]; then
    executar
fi

if [ "$TEST" -eq "1" ]; then
    testar
fi
