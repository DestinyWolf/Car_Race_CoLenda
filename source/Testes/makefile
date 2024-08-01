all: multiplayer

game_multiplayer: multiplayer
game_singleplayer: single_player

multiplayer: main_test_multijogador.c mouse_module.o obstacle.o background_animation_module.o create_sprite.o colision_module.o colenda.o keys.o display_7seg.o create_cover.o
	gcc -o multiplayer main_test_multijogador.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o keys.o display_7seg.o create_cover.o create_sprite.o -pthread -std=c99

single_player: main_test.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o keys.o display_7seg.o create_cover.o create_sprite.o
	gcc -o single_player main_test.c mouse_module.o obstacle.o background_animation_module.o colision_module.o colenda.o keys.o display_7seg.o create_cover.o create_sprite.o -pthread -std=c99


mouse_module.o: mouse_module.c mouse_module.h
	gcc -c mouse_module.c  -std=c99

create_sprite.o: create_sprite.c create_sprite.h
	gcc -c create_sprite.c -std=c99

obstacle.o: obstacle.c obstacle.h
	gcc -c obstacle.c -std=c99

background_animation_module.o: background_animation_module.c background_animation_module.h
	gcc -c background_animation_module.c -std=c99

colision_module.o: colision_module.c colision_module.h
	gcc -c colision_module.c -std=c99

colenda.o: ../Lib/colenda.c ../Lib/colenda.h
	gcc -c ../Lib/colenda.c -std=c99

keys.o: ../drivers/pushbuttons/keys.c ../drivers/pushbuttons/keys.h
	gcc -c ../drivers/pushbuttons/keys.c -std=c99

display_7seg.o: ../drivers/7seg_display/display_7seg.c ../drivers/7seg_display/display_7seg.h
	gcc -c ../drivers/7seg_display/display_7seg.c -std=c99

create_cover.o: create_cover.c create_cover.h
	gcc -c create_cover.c -std=c99

create_sprite.o: create_sprite.c create_sprite.h
	gcc -c create_sprite.c -std=c99

clean:
	rm -r *.o 
