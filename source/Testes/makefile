all: multiplayer

game_multiplayer: multiplayer
game_singleplayer: single_player

multiplayer: main_test_multijogador.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o
	gcc -o multiplayer main_test_multijogador.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o -pthread -std=c99

single_player: main_test.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o
	gcc -o single_player main_test.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o -pthread -std=c99


mouse_module.o: mouse_module.c mouse_module.h
	gcc -c mouse_module.c  -std=c99

obstacle.o: obstacle.c obstacle.h
	gcc -c obstacle.c -std=c99

background_animation_module.o: background_animation_module.c background_animation_module.h
	gcc -c background_animation_module.c -std=c99

colision_module.o: colision_module.c colision_module.h
	gcc -c colision_module.c -std=c99

colenda.o: ../Lib/colenda.c ../Lib/colenda.h
	gcc -c ../Lib/colenda.c -std=c99

clean:
	rm -r *.o 