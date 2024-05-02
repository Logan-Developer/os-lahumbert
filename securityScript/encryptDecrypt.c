#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void usage(char *argv[]) {
    printf("Usage: %s [-e|-d] [-k <publicKeyfile>] [-p <privateKeyfile>] [-o <outputfile>] <inputfile>\n", argv[0]);
    printf("  -e: encrypt\n");
    printf("  -d: decrypt\n");
    printf("  -k: public key file\n");
    printf("  -p: private key file\n");
    printf("  -o: output file\n");
    printf("  <inputfile>: file to encrypt/decrypt\n");

    exit(1);
}

char *mode = "enc";
char *publicKeyFile = "public.pem";

char *privateKeyFile = "private.pem";

char *outputFile = "output";

void checkArgs(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv);
    }

    int i;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'e':
                    mode = "enc";
                    break;
                case 'd':
                    mode = "dec";
                    break;
                case 'k':
                    i++;
                    if (i >= argc) {
                        usage(argv);
                    }
                    publicKeyFile = argv[i];
                    break;
                case 'p':
                    i++;
                    if (i >= argc) {
                        usage(argv);
                    }
                    privateKeyFile = argv[i];
                    break;
                case 'o':
                    i++;
                    if (i >= argc) {
                        usage(argv);
                    }
                    outputFile = argv[i];
                    break;
                default:
                    usage(argv);
            }
        } else {
            break;
        }
    }

}

void createKeyPair() {
    printf("Creating new key pair\n");

    // Create a new key pair with openssl
    if (fork() == 0) {
        execlp("openssl", "openssl", "genrsa", "-out", privateKeyFile, "2048", NULL);
    }

    wait(NULL);

    if (fork() == 0) {
        execlp("openssl", "openssl", "rsa", "-in", privateKeyFile, "-outform", "PEM", "-pubout", "-out", publicKeyFile, NULL);
    }

    wait(NULL);
}

void encryptFile(char *inputFile, char *publicKeyFile) {
    printf("Encrypting file %s with public key %s\n", inputFile, publicKeyFile);

    // if key not found, create a new key pair
    if (access(publicKeyFile, F_OK) == -1) {
        printf("Key not found, creating new key pair\n");
        createKeyPair();
    }

    // Encrypt the file with openssl
    if (fork() == 0) {
        execlp("openssl", "openssl", "rsautl", "-encrypt", "-inkey", publicKeyFile, "-pubin", "-in", inputFile, "-out", outputFile, NULL);
    }

    wait(NULL);
}

void decryptFile(char *inputFile, char *privateKeyFile) {
    printf("Decrypting file %s with private key %s\n", inputFile, privateKeyFile);

    // if key not found, cancel
    if (access(privateKeyFile, F_OK) == -1) {
        printf("Private key not found, aborting\n");
        exit(1);
    }

    // Decrypt the file with openssl
    if (fork() == 0) {
        execlp("openssl", "openssl", "rsautl", "-decrypt", "-inkey", privateKeyFile, "-in", inputFile, "-out", outputFile, NULL);
    }

    wait(NULL);
}

int main (int argc, char *argv[]) {
    checkArgs(argc, argv);

    if (mode == "enc") {
        encryptFile(argv[argc - 1], publicKeyFile);
    } else {
        decryptFile(argv[argc - 1], privateKeyFile);
    }

    return 0;
}
