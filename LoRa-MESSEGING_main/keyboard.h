//Pins clavier-rp2040
int keybC[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
#define r0 10
#define r1 11
#define r2 12
int ran = 2;
int pause = 300;
int keyi = 0;
int keyj = 0;
//3 keyboards of 3 by 10
char keyb[9][10] = {
  { 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' },
  { 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm' },
  { '\a', 'w', 'x', 'c', ' ', 'v', 'b', 'n', '\n', '\b' },
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' },
  { '\f', ',', '.', '#', ':', '?', '!', '-', '+', '=' },
  { '\a', '\v', '\t', '@', ' ', '*', '/', '&', '\n', '\b' },
  { 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' },
  { 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M' },
  { '\a', 'W', 'X', 'C', ' ', 'V', 'B', 'N', '\n', '\b' },
};

void keybR(uint8_t i) {
  switch (i) {
    case 0:  // activation de la ligne 1
      digitalWrite(r0, LOW);
      digitalWrite(r1, HIGH);
      digitalWrite(r2, HIGH);
      break;
    case 1:  // activation de la ligne 2
      digitalWrite(r0, HIGH);
      digitalWrite(r1, LOW);
      digitalWrite(r2, HIGH);
      break;
    case 2:  // activation de la ligne 3
      digitalWrite(r0, HIGH);
      digitalWrite(r1, HIGH);
      digitalWrite(r2, LOW);
      break;
  }
}

//Lecture du clavier
char readKeyboard(bool Shift) {

  char boutton = '\0';
  keyj = 100;
  keyi = 100;

  for (uint8_t i = 0; i < 3; ++i) {
    keybR(i);  //Rows
    for (uint8_t j = 0; j < 10; ++j) {
      if (digitalRead(keybC[j]) == LOW) {
        //keyi = i;
        //keyj = j;
        if (ran == 2) {
          i = i + 3;
        } else if (ran == 3) {
          i = i + 6;
        }
        boutton = keyb[i][j];
        delay(pause);
        break;
      }
    }
  }
  return boutton;
}
