# A* 

## Librerias

- Instalar SFML 
```bash
sudo apt update
sudo apt install libsfml-dev
```

## Comando para compilar el programa

```bash
g++ main.cpp Astar.cpp Render.cpp -o main \
-lsfml-graphics \
-lsfml-window \
-lsfml-system
```

## Input de prueba

```bash
5
6
7
0
2
2
2
3
2
4
2
2
4
3
4
4
4
0
0
4
5
1
41
32
25
20
17
16
34
25
18
13
10
9
29
20
13
8
5
4
26
17
10
5
2
1
25
16
9
4
1
0
```


## Resultado esperado
<img width="706" height="567" alt="image" src="https://github.com/user-attachments/assets/3485ba14-383c-4b66-9dfe-0b674035fabe" />


