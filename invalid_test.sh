#!/home/grzegorz/Documents/SimpleShell/build/SimpleShell

# próba odczytu z pliku, który nie istnieje
nie_istnieje>0 wc -l
# wyrażenie, w którym zapomniano ; po cd
cd .. ls
# podwójny potok
ls -l || grep 'main' # nie wspieramy operatora ||
# niepoprawne wyrażenie przekierowania
ls -l plik1>plik2
# zły deskryptor strumienia
ls -l 1>3
# brak nawiasu
( cd .. ; ( cd .. ; ls -l )
# zepsuty string
ls -l | grep 'main | wc -l
