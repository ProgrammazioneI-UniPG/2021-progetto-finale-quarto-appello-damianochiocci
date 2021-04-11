#include "gamelib.h"

int main() {
  printf("*-*-*-*-*-WELCOME TO...TRANNOI!-*-*-*-*-*\n");
  do {
      if (!confirm_setting) {
        printf("\n1) Set Game\n2) Play\n3) End Game\n");
        scanf("%d", &choice);
        while (getchar()!='\n');
        switch(choice) {
          case 1:
            Set_Game();
            break;
          case 2:
            printf ("You haven't already set the game!\n");
            break;
          case 3:
            End_Game();
            break;
          default:
            printf("Choice not suitable!\n");
            break;
        }
      } else if (confirm_setting && !end_of_game) {
        printf("You set the game, now you can:\n2) Play\n3) End Game\n");
        scanf("%d", &choice);
        while (getchar()!='\n');
        switch(choice) {
          case 2:
            Play();
            break;
          case 3:
            End_Game();
            break;
          default:
            printf("Choice not suitable!\n");
            break;
        }
      } else if (end_of_game) {
        printf ("The game is finished, press 3 to end game\n");
        scanf("%d", &choice);
        while (getchar()!='\n');
        switch (choice) {
          case 3:
            End_Game();
            break;
          default:
            printf ("Choice not suitable!\n");
            break;
        }
      }
  } while(choice !=3);
  return 0;
}
