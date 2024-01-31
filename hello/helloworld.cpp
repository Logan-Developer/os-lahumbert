#include <iostream>
#include <string>
#include <unistd.h>  // Add this header for getpid()

// Function to encrypt and decrypt a string using XOR
std::string encryptDecrypt(const std::string& input, const char key) {
    std::string output = input;
    for (char &c : output) {
        c ^= key;
    }
    return output;
}

int main() {
    // Get the process ID
    pid_t processId = getpid();

    // Original message
    std::string originalMessage = "Hello, World!";

    // Encryption key
    char encryptionKey = 'K';

    // Encrypt the message
    std::string encryptedMessage = encryptDecrypt(originalMessage, encryptionKey);

    // Display the process ID
    std::cout << "Process ID: " << processId << std::endl;

    // Display the encrypted message
    std::cout << "Encrypted Message: " << encryptedMessage << std::endl;

    // Decrypt the message
    std::string decryptedMessage = encryptDecrypt(encryptedMessage, encryptionKey);

    // Display the decrypted message
    std::cout << "Decrypted Message: " << decryptedMessage << std::endl;

    return 0;
}
