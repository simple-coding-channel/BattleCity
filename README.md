# BattleCity

Проект канала [SimpleCoding](https://www.youtube.com/watch?v=wf37ukItjE8&list=PL6x9Hnsyqn2XU7vc8-oFLojbibK91fVd-&index=1&ab_channel=SimpleCoding) для обучения создания простого 2Д игрового движка на OpenGL

## Сборка
Проект планируется быть кроссплатформенным, в настоящее время протестирован на Windows, Linux и Mac.
Для сборки необходимы только Git, CMake и компилятор

```powershell
git clone https://github.com/simple-coding-channel/BattleCity.git
cd BattleCity
mkdir build
cmake ..
cmake --build .
```

Под Ubuntu возможно нужно будет еще установить следующую зависимость:
```powershell
sudo apt-get install xorg-dev libglu1-mesa-dev
```
