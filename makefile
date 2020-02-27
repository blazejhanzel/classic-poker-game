compiler=g++
standard=-std=c++17
debug=-g
bin=bin/poker.exe
include=-Iinclude/
lib=-Llib/ -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -static-libgcc -static-libstdc++ -lpthread

all: obj/main.o obj/AI.o obj/Card.o obj/CGame.o src/CLog.h obj/CUI.o obj/Human.o obj/Player.o obj/Round.o obj/Sequence.o obj/Table.o obj/Token.o obj/UI.o obj/MainMenu.o obj/GamePlay.o obj/LoadingScreen.o obj/GameOver.o
	$(compiler) $(standard) $(debug) -o $(bin) obj/*.o $(include) $(lib)

obj/main.o: src/main.cpp src/CUI.h
	$(compiler) $(standard) $(debug) -o obj/main.o -c src/main.cpp $(include) $(lib)

obj/AI.o: src/AI.cpp src/AI.h src/Player.h
	$(compiler) $(standard) $(debug) -o obj/AI.o -c src/AI.cpp $(include) $(lib)

obj/Card.o: src/Card.cpp src/Card.h src/CLog.h
	$(compiler) $(standard) $(debug) -o obj/Card.o -c src/Card.cpp $(include) $(lib)

obj/CGame.o: src/CGame.cpp src/CGame.h src/CLog.h src/Player.h src/Human.h src/AI.h src/Token.h src/Card.h src/Table.h src/Round.h
	$(compiler) $(standard) $(debug) -o obj/CGame.o -c src/CGame.cpp $(include) $(lib)

obj/CUI.o: src/CUI.cpp src/CUI.h src/CGame.h src/CLog.h src/UI/UI.h src/UI/MainMenu.h src/UI/GamePlay.h src/UI/LoadingScreen.h src/UI/GameOver.h src/Human.h
	$(compiler) $(standard) $(debug) -o obj/CUI.o -c src/CUI.cpp $(include) $(lib)

obj/Human.o: src/Human.cpp src/Human.h src/Player.h src/UI/UI.h
	$(compiler) $(standard) $(debug) -o obj/Human.o -c src/Human.cpp $(include) $(lib)

obj/Player.o: src/Player.cpp src/Player.h src/CLog.h src/Card.h src/Token.h
	$(compiler) $(standard) $(debug) -o obj/Player.o -c src/Player.cpp $(include) $(lib)

obj/Round.o: src/Round.cpp src/Round.h src/CLog.h src/Table.h src/Player.h src/Card.h src/Sequence.h
	$(compiler) $(standard) $(debug) -o obj/Round.o -c src/Round.cpp $(include) $(lib)

obj/Sequence.o: src/Sequence.cpp src/Sequence.h src/Card.h src/CLog.h
	$(compiler) $(standard) $(debug) -o obj/Sequence.o -c src/Sequence.cpp $(include) $(lib)

obj/Table.o: src/Table.cpp src/Table.h src/Card.h src/Token.h src/CLog.h
	$(compiler) $(standard) $(debug) -o obj/Table.o -c src/Table.cpp $(include) $(lib)

obj/Token.o: src/Token.cpp src/Token.h src/CLog.h
	$(compiler) $(standard) $(debug) -o obj/Token.o -c src/Token.cpp $(include) $(lib)

obj/UI.o: src/UI/UI.cpp src/UI/UI.h
	$(compiler) $(standard) $(debug) -o obj/UI.o -c src/UI/UI.cpp $(include) $(lib)

obj/MainMenu.o: src/UI/MainMenu.cpp src/UI/MainMenu.h src/UI/UI.h
	$(compiler) $(standard) $(debug) -o obj/MainMenu.o -c src/UI/MainMenu.cpp $(include) $(lib)

obj/GamePlay.o: src/UI/GamePlay.cpp src/UI/GamePlay.h src/UI/UI.h
	$(compiler) $(standard) $(debug) -o obj/GamePlay.o -c src/UI/GamePlay.cpp $(include) $(lib)

obj/LoadingScreen.o: src/UI/LoadingScreen.cpp src/UI/LoadingScreen.h src/UI/UI.h
	$(compiler) $(standard) $(debug) -o obj/LoadingScreen.o -c src/UI/LoadingScreen.cpp $(include) $(lib)

obj/GameOver.o: src/UI/GameOver.cpp src/UI/GameOver.h src/UI/UI.h
	$(compiler) $(standard) $(debug) -o obj/GameOver.o -c src/UI/GameOver.cpp $(include) $(lib)

clean:
	del obj\*