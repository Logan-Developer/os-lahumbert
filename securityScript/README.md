# Bash script to encrypt / decrypt data using public / private key encryption

## Usage

```bash
./encyptDecrypt.sh [-e|-d] [-k <keyfile>] [-o <outputfile>] <inputfile>
```

## Options

- `-e` : Encrypt the input file
- `-d` : Decrypt the input file
- `-k <keyfile>` : Use the keyfile for encryption / decryption
- `-o <outputfile>` : Write the output to the outputfile