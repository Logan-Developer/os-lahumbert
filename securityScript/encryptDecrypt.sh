#!/bin/bash

function usage() {
    echo "Usage: $0  [-e|-d] [-k <publicKeyfile>] [-p <privateKeyfile>] [-o <outputfile>] <inputfile>"
    echo "    -e: encrypt"
    echo "    -d: decrypt"
    echo "    -k: public key file"
    echo "    -p: private key file"
    echo "    -o: output file"
    exit 1

}

if [ $# -eq 0 ]; then
    usage
fi

MODE="enc"
PUBLIC_KEY=""
PRIVATE_KEY=""

while getopts "edk:p:o:" opt; do
    case $opt in
        e)
            MODE="enc"
            ;;
        d)
            MODE="dec"
            ;;
        k)
            PUBLIC_KEY=$OPTARG
            ;;
        p)
            PRIVATE_KEY=$OPTARG
            ;;
        o)
            OUTPUT_FILE=$OPTARG
            ;;
        *)
            usage
            ;;
    esac
done

shift $((OPTIND - 1))

if [ -z "$OUTPUT_FILE" ]; then
    if [ "$MODE" == "enc" ]; then
        OUTPUT_FILE="$1.enc"
    else
        OUTPUT_FILE="$1.dec"
fi


# If no key is provided, create a new key pair
if [ -z "$PUBLIC_KEY" ] || [ -z "$PRIVATE_KEY" ]; then
    if [ -f "public.pem" ] && [ -f "private.pem" ]; then
        PUBLIC_KEY="public.pem"
        PRIVATE_KEY="private.pem"
    else
        echo "No key provided, creating a new key pair"
        openssl genrsa -out private.pem 2048
        openssl rsa -in private.pem -outform PEM -pubout -out public.pem
        PUBLIC_KEY="public.pem"
        PRIVATE_KEY="private.pem"
    fi
fi

echo "Public key: $PUBLIC_KEY"
echo "Private key: $PRIVATE_KEY"
echo "Output file: $OUTPUT_FILE"
echo "Input file: $1"
echo "Mode: $MODE"

if [ "$MODE" == "enc" ]; then
    echo "Encrypting $1"
    openssl rsautl -encrypt -inkey $PUBLIC_KEY -pubin -in $1 -out $OUTPUT_FILE
else
    echo "Decrypting $1"
    openssl rsautl -decrypt -inkey $PRIVATE_KEY -in $1 -out $OUTPUT_FILE
fi