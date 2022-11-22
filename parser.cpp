#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

using namespace std;

static const unsigned int MAX_BUFFER_SIZE = 16;
static const unsigned int MAX_MESSAGES = 2;

struct message_header {
  long length;
};

extern "C" void win();

void win() {
  system("/bin/sh");
}

void printMenu() {
  cout << endl;
  cout << "(p) Parse message" << endl;
  cout << "(s) Show message length" << endl;
  cout << "(q) Quit" << endl;
  cout << "> ";
}

int main(int argc, char const *argv[]) {
  struct message_header messageArray[MAX_MESSAGES] = {{0}, {0}};
  size_t numMessages = 0;

  while (true) {
    printMenu();

    char action;
    cin >> action;
    cin.ignore(); // Ignore newline

    switch (action) {
      case 'p':
      {
        if (numMessages >= MAX_MESSAGES) {
          cout << "You've parsed too many messages!" << endl;
          break;
        }

        cout << "Please enter message in format 'length (1 digit) + data' (ex. 5AAAA): " << endl;

        struct message_header hdr;
        char buffer[MAX_BUFFER_SIZE] = {0};
        char lengthChar;

        if (fread(&lengthChar, sizeof(char), 1, stdin) < 0) {
          cout << "Error: Length read failed" << endl;
          break;
        }

        // Convert length digit to integer
        int len = lengthChar - '0';
        cout << "Successfully parsed message len: " << len << endl;

        // Check length of buffer
        if (len > (MAX_BUFFER_SIZE - sizeof(char))) {
          cout << "Error: Length is larger than MAX_BUFFER_SIZE " << MAX_BUFFER_SIZE << endl;
          break;
        }

        // Read data of length bytes into buffer
        if (fread(buffer, len - sizeof(char), 1, stdin) < 0) {
          cout << "Error: Data read failed" << endl;
          break;
        }

        cout << "Successfully parsed data: " << buffer << endl;

        hdr.length = (long)len;

        messageArray[numMessages] = hdr;
        numMessages++;
        break;
      }
      case 's':
      {
        cout << "What index?" << endl;
        int idx;
        cin >> idx;

        // Check that idx is in range
        if (idx < 0 || MAX_MESSAGES - idx <= 0) {
          cout << "Invalid index" << endl;
          break;
        }

        if (messageArray[idx].length != 0) {
          cout << "length: " << hex << messageArray[idx].length << endl;
        } else {
          cout << "Message does not exist" << endl;
        }
        break;
      }
      case 'q':
        cout << "Goodbye!" << endl;
        return 0;
      default:
        cout << "Unrecognized option " << action << endl;
    }
  }
}