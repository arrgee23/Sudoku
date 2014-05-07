#include "sudoku.h"

GtkWidget *window2;

// warns if bad number
bool bad_number(int i)
{
	if(!(i>=0 && i<10))
	{
		open_dialog("Error","This number is bigger than 9 or less than 0\nOr maybe not even a number");
		return 1;
	}
	return 0;
}



// dialog function
void open_dialog(const char* msgtype,const char* msgname)
{
	GtkWidget* dialog,*label;
	dialog=gtk_dialog_new_with_buttons(msgtype,GTK_WINDOW(window2),GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	label=gtk_label_new(msgname);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,0,0,0);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

bool checkall(int i,int j)
{
	return checkrow(i,j) && checkcol(i,j) && checkbox(i,j);
}

// function that returns a user selected number
// displays a window with buttons to select numbers
int get_number_from_input_with_buttons(int x,int y)
{
	GtkWidget *button;
	window2 = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_title(GTK_WINDOW(window2),"INPUT");
	gtk_window_set_default_size(GTK_WINDOW(window2),40,40);
	GtkWidget* table;
	table = gtk_table_new(3,3,FALSE);
	
	int i,j;
	int count=0;
	int return_value;
	char button_label[2];
	for(i=0;i<3;++i)
		for(j=0;j<3;j++)
		{
			sprintf(button_label,"%d",++count);
			button = gtk_button_new_with_label(button_label);
			
			gtk_table_attach_defaults(GTK_TABLE(table),button,j,j+1,i,i+1);
			//callback func here
			g_signal_connect (G_OBJECT (button), "clicked",G_CALLBACK (button_press), &return_value);
		}
		gtk_container_add(GTK_CONTAINER(window2),table);
		gtk_widget_show_all(window2);
		
		int window_x,window_y;
		gtk_window_get_position(GTK_WINDOW(window),&window_x,&window_y);
		gtk_window_move(GTK_WINDOW(window2),x+ window_x,y+ window_y);
		gtk_main();
		return return_value;
}

void button_press(GtkWidget* button,GdkEventButton *event,int* count)
{
	const char* c = gtk_button_get_label(GTK_BUTTON(button));
	*count = atoi(c);
	gtk_widget_destroy(window2);
	gtk_main_quit();
	is_label_clicked = false;
}

bool checkrow(int s,int o)
{
	int checkarray[10] = {0};
	for(int g=0;g<9;g++)
	{
		++checkarray [a[s][g]];
	}
	
	for(int g=1;g<10;g++)
	{
		if(checkarray[g] > 1)
		{
			//printf("row");
			return 0;
		}
	}
	return 1;
}

bool checkcol(int s,int o)
{
	int checkarray[10] = {0};
	for(int g=0;g<9;g++)
	{
		checkarray [a[g][o]]++;
	}
	
	for(int g=1;g<10;g++)
	{
		if(checkarray[g] > 1)
		{
			//printf("col");
			return 0;
		}
	}
	return 1;
}

bool checkbox(int s,int o)
{
	int checkarray[10] = {0};
	//int index = 0;
	
	int checkrow=ceil((s+1)/3.); // ceil returns the lowest int value that is greater than the passed float 
	int checkcol=ceil((o+1)/3.);
	
	for(int g=(checkrow-1)*3;g<((checkrow-1)*3+3);g++)
		for(int h=(checkcol-1)*3;h<((checkcol-1)*3+3);h++)
		{
			checkarray[a[g][h]]++;
		}
	for(int g=1;g<10;g++)
	{
		if(checkarray[g] > 1)
		{
			//printf("box");
			return 0;
		}
	}
	return 1;
}


void print()
{
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++)
			printf("%d ",a[i][j]);
		puts("");
	}
}

// loads the matrix from entry
void get_elements()
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			a[i][j]=atoi(gtk_label_get_text(GTK_LABEL(label[i][j])));
}

// matrix -> entry
void set_elements()
{
	GdkColor color;
	bool default_cell = false;
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			if(a[i][j])
			{
				for (int k = 0; k < read_pairs.size(); k++)
				{
					if(read_pairs[k].x == i && read_pairs[k].y == j)
						default_cell = true;
				}
				if(!default_cell)
					set_text_single_cell(i,j,"green");
				else
				{
					set_text_single_cell(i,j,"purple");
					default_cell = false;
				}
			}
			else
			{
				gdk_color_parse ("grey", &color);
				gtk_widget_modify_bg (event_box[i][j], GTK_STATE_NORMAL, &color);
				gtk_label_set_text(GTK_LABEL(label[i][j]),"");
			}
}

void check_and_mark(int x,int y)
{
	int rowindex =  x;
	int colindex =  y;
	int number = a[rowindex][colindex];
	
	// check in row and col
	for(int g=0;g<9;g++)
	{
		if(a[rowindex][g] == number && g != colindex && number !=0 && a[rowindex][g] !=0)
		{
			warn_for_duplicate(rowindex,colindex);	
			warn_for_duplicate(rowindex,g);	
		}
	}
	
	for (int g = 0; g < 9; g++)
	{
		if(a[g][colindex] == number && g != rowindex && number !=0 && a[g][colindex] != 0)
		{
			warn_for_duplicate(rowindex,colindex);
			warn_for_duplicate(g,colindex);
		}
	}
	
	int checkrow=ceil((rowindex+1)/3.); // ceil returns the lowest int value that is greater than the passed float 
	int checkcol=ceil((colindex+1)/3.);
	
	for(int g=(checkrow-1)*3;g<((checkrow-1)*3+3);g++)
		for(int h=(checkcol-1)*3;h<((checkcol-1)*3+3);h++)
		{
			if(a[g][h] == number && g != rowindex && h != colindex && number !=0 && a[g][h] != 0)
			{
				warn_for_duplicate(rowindex,colindex);
				warn_for_duplicate(g,h);
			}
		}
}

void warn_for_duplicate(int x,int y)
{
	GdkColor color;
	
	gdk_color_parse ("red", &color);
	gtk_widget_modify_bg (event_box[x][y], GTK_STATE_NORMAL, &color);
}

// sets the cell with given color in the given coordinate
void set_text_single_cell(int i,int j,char* cellcolor)
{
	GdkColor color;
	char c[2];
	sprintf(c,"%d",a[i][j]);
	gtk_label_set_text(GTK_LABEL(label[i][j]),c);
	
	gdk_color_parse (cellcolor, &color);
	gtk_widget_modify_bg (event_box[i][j], GTK_STATE_NORMAL, &color);
	
	gdk_color_parse ("white", &color);
	gtk_widget_modify_fg (label[i][j], GTK_STATE_NORMAL, &color);
}

void newgame()
{
	for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				a[i][j] = 0;
		
	ifstream in("sudoku.txt");
	read_pairs.clear();
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if(in.eof())
				{	printf("eof\n");
					break;
				}	
				in >> a[i][j];
				
				if(a[i][j])
				{
					mypair temppair(i,j);
					read_pairs.push_back(temppair);
				}
			}	
		}
	in.close();
	set_elements();
}
