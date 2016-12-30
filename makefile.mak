CXXFLAGS= -O3 -finline-functions -std=c++11 -O3 -I../tasklib `pkg-config gtkmm-3.0 --cflags`
LDFLAGS= -L../tasklib `pkg-config sfml-all --libs` `pkg-config gtkmm-3.0 --libs` -lmysqlcppconn -pthread
OBJS= Task.o Conditions.o Scenario.o Database.o Gui.o Media.o
