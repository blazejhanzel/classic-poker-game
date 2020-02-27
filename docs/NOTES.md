# Projekt
## Algorytm gry
1. Przygotowanie talii kart (52 karty)
2. Gracze wnoszą opłatę "ante"
3. Rozdanie każdemu z graczy po 5 kart
4. Licytacja (sprawdzenie, spasowanie, podbicie)
5. Wymiana kart przez każdego z graczy zaczynając od osoby siedzącej na lewo od Dealera
6. Licytacja (do momentu wyrównania wszystkich zakładów)
7. Sprawdzenie wyników

## Algorytm CGame
1. Przygotowanie miejsc dla określonej ilości graczy i zdefiniowanie ich typu (komputer / człowiek)
2. Wynegerowanie gotówki startowej oraz rzetonów startowych
3. Rozdanie każdemu graczowi określonej ilości gotówki startowej oraz rzetonu typu dealer
4. Przygotowanie talii 52 kart
5. Zainicjowanie stołu (jednorazowo)
6. Zainicjowanie rundy (wskazanie na stół na którym odbędzie się gra)

## Algorytm Round
1. Pobranie na stół opłaty ante od każdego gracza i wyłączenie z rundy graczy, którym zabrakło gotówki i wyłożenie tej opłaty na stół
2. Przetasowanie kart
3. Rozdanie każdemu z graczy po 5 kart
4. Wyłożenie pozostałych kart na stół
5. !! Przeprowadzenie akcji dla każdego gracza (licytacja)
6. Wymiana kart przez graczy
7. Licytacja, aż do wyrównania wkładu
8. Sprawdzenie kart i wyłonienie zwycięzcy
9. Wręczenie zwycięzcy wygranych żetonów
