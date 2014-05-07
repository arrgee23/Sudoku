#ifndef SUDOKU_H   /* Include guard */
#define SUDOKU_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <gtk/gtk.h>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;
// "button-press-event"

class mypair{
	public:
	mypair(){}
	mypair(int i,int j) : x(i),y(j){};
	int x,y;
};

extern int a[9][9];
extern GtkWidget* label[9][9],*window,*event_box[9][9];
extern vector <mypair> read_pairs;
extern bool is_label_clicked;

void set_text_single_cell(int i,int j,char* color);
int get_number_from_input_with_buttons(int x,int y);
void set_elements();
void print();
void menu_event(GtkWidget*,gpointer);
void label_clicked(GtkWidget* widget,GdkEventButton* event,mypair* data);
void solve_button(GtkWidget* entry,gpointer data);
void next_button(GtkWidget* entry,gpointer data);
void open_dialog(const char* msgtype,const char* msgname);
void get_elements();
bool checkrow(int s,int o);
bool checkcol(int s,int o);
bool checkbox(int s,int o);
bool checkall(int i,int j);
bool solve();
void check_and_mark(int x,int y);
void warn_for_duplicate(int x,int y);
bool backtrack(int index);
void newgame();
void button_press(GtkWidget* button,GdkEventButton *event,int* count);

#endif
