
# app.exe: obj/*.o ../WIZ/obj/*.o
# 	c++ $< -o app.exe -L../libs -I../include -I.

# obj/*.o: ../WIZ/obj/*.o
# 	c++ -c -o $@ $<

FLAGS := -std=c++11 -Wall -I../WIZ/include

objects := $(patsubst %.cpp,obj/%.o, $(wildcard *.cpp))

headers := $(wildcard *.h)
wizheaders := $(wildcard ../WIZ/include/WIZ/*.h)

# wizobjects := $(patsubst %.cx,%.o,$(wildcard ../WIZ/obj/*.o))

app: $(objects) wizlib
	c++ -o game.exe $(objects) -I../include -LC:\games\libs -lWIZ -lglew32 -lglfw3 -lSOIL -lopengl32 -lgdi32

obj/%.o: %.cpp $(headers) $(wizheaders)
	@echo compiling $@
	c++ -c $< -o $@ -I../include $(FLAGS)

# $(wizobjects):
# c++ -c wizobjects ../WIZ/obj/$@ -I../include

wizlib:
	make -C ../WIZ