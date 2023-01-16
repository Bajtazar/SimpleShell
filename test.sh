#!/home/grzegorz/Documents/SimpleShell/build/SimpleShell

# Proste echo
echo 'Pierwszy test'
echo 'test'
# Grupa
echo 'Drugi test'
( cd .. ; pwd )
pwd
# Pipe
echo 'Trzeci test'
ls -l | grep 'main'
# Grupa z pipem
echo 'Czwarty test'
( cd .. ; ls -l | grep 'proj' )
# Zaawansowana grupa
echo 'Piąty test'
( cd .. ; ls -l | grep 'proj' ) | wc -l
# Proste przekierowanie do pliku
echo 'Szósty test'
( cd .. ; ls -l | grep 'proj' ) >res.txt
cat res.txt | wc -l ; rm res.txt
# Zagnieżdżona grupa
echo 'Siódmy test'
(cd .. ; ( cd .. ; ls -l ) | grep 'wino' )
# Przekierowanie do pliku w środku potoku
echo 'Ósmy test'
ls -l | grep 'main' >res.txt | wc -l
cat res.txt
rm res.txt
# Zaawansowane przekierowania
echo 'Dziewiąty test'
ls -l 2>plik.txt 1>&2
cat plik.txt
rm plik.txt
# Zaawansowane przekierowania
echo 'Dziesiąty test'
ls -l 2>plik.txt 1>2
ls | grep 'plik.txt' | wc -l
rm plik.txt
# Echo z nową linią
echo 'Jedenasty \n test'
