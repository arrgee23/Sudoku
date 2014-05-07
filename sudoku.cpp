#include "sudoku.h"

bool is_label_clicked = false;
int a[9][9];
GtkWidget* label[9][9],*window,*event_box[9][9];
vector <mypair> coordinate;
vector <mypair> read_pairs;

int main(int argc,char* argv[])
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			a[i][j]=0;
	const char* file[] = {"Open","Check"};
	
	gtk_init(&argc,&argv);
	
	GtkWidget *vbox,*hbox,*separator,*button,*button2,*menu,*file_menu,*menu_bar;
	vbox=gtk_vbox_new(0,0);
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	menu=gtk_menu_new();
	menu_bar=gtk_menu_bar_new();
	file_menu=gtk_menu_item_new_with_label("File");
	
	// add menushells
	for(int i=0;i<2;i++)
	{
		GtkWidget* menu_item = gtk_menu_item_new_with_label(file[i]);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_item);
		g_signal_connect(menu_item,"activate",G_CALLBACK(menu_event),window); // callback menu event
	}
	// set them as submenu to file
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu),menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),file_menu);
	gtk_box_pack_start(GTK_BOX(vbox),menu_bar,0,0,0);
	
	// pack entryfield and separators
	mypair pair[9][9];
	
	for(int i=0;i<9;i++)
	{
		hbox=gtk_hbox_new(0,0);
		for(int j=0;j<9;j++)
		{
			pair[i][j].x = i;
			pair[i][j].y = j;
			event_box[i][j] = gtk_event_box_new ();
			label[i][j]=gtk_label_new("0");
			
			gtk_container_add (GTK_CONTAINER (event_box[i][j]), label[i][j]);
			gtk_widget_set_events (event_box[i][j], GDK_BUTTON_PRESS_MASK);
			g_signal_connect(GTK_OBJECT(event_box[i][j]),"button-press-event",G_CALLBACK(label_clicked),&pair[i][j]); // event
			gtk_label_set_width_chars(GTK_LABEL(label[i][j]),1);
			gtk_label_set_max_width_chars (GTK_LABEL(label[i][j]),1);
			gtk_widget_set_size_request(label[i][j],40,40);
			
			gtk_box_pack_start(GTK_BOX(hbox),event_box[i][j],0,0,2);
			if((j+1)%3==0)
			{
				separator=gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,1);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,2);
		if((i+1)%3==0)
		{
			separator=gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,1);
		}
	}
	
	// make solve button
	button=gtk_button_new_with_label("Solve the sudoku");
	g_signal_connect(button,"clicked",G_CALLBACK(solve_button),NULL);
	gtk_box_pack_start(GTK_BOX(vbox),button,0,0,0);
	
	button=gtk_button_new_with_label("Next");
	g_signal_connect(button,"clicked",G_CALLBACK(next_button),NULL);
	gtk_box_pack_start(GTK_BOX(vbox),button,0,0,0);
	
	g_signal_connect(window,"delete-event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	
	gtk_widget_show_all(window);
	newgame();
	gtk_main();
	return 0;
}

void menu_event(GtkWidget* menu_item,gpointer data)
{
	
	// if open is clicked open a filechooser
	// to choose a text that contais the board
	if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)),"Open")==0)
	{
		GtkWidget *dialog;
		dialog=gtk_file_chooser_dialog_new("Choose the sudoku file",GTK_WINDOW(data),GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_OK,GTK_RESPONSE_OK,
		GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
		
		gtk_widget_show_all(dialog);
		gint resp=gtk_dialog_run(GTK_DIALOG(dialog));
		if(resp==GTK_RESPONSE_OK)
		{
			for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				a[i][j] = 0;
				
			// get filename and open file
			ifstream in(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
			read_pairs.clear();
			for(int i=0;i<9;i++)
			{
				for(int j=0;j<9;j++)
				{
					if(in.eof())
						break;
						
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
		gtk_widget_destroy(dialog);
	}
	
	// check the board if check is clicked
	else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)),"Check")==0)
	{
		get_elements();
		bool wrong=false;
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				if( !( a[i][j]>0 && a[i][j]<10 && checkall(i,j) ) )
				{
					wrong=true;
				}
		
		if(wrong)// make a dialog box with warning
			open_dialog("Error","Wrong!!\n Check Inputs Again!!");
		else // ok
			open_dialog("Success","Yahoo!! way to go sudoku master");
	}
	
}

void label_clicked(GtkWidget* widget,GdkEventButton* event,mypair* data)
{
	if(!is_label_clicked)
	{
		is_label_clicked = true;
		get_elements();
		for (int k = 0; k < read_pairs.size(); k++)
		{
			if(read_pairs[k].x == data->x && read_pairs[k].y == data->y)
			{
				is_label_clicked = false;
				return;
			}
		}
		// for leftclick take input
		if(event->button == 1)
		{
			char c[2];
			sprintf(c,"%d",get_number_from_input_with_buttons(data->x*40 + (int)event->x,data->y*40 + (int)event->y));
			
			if(!isdigit(c[0])) // if not digit warn
				open_dialog("Error","This is not a number!");
			else
			{
				a[data->x][data->y]=atoi(c);
				set_elements();
				//check_and_mark(data->x,data->y);
				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						check_and_mark(i,j);
				
			}
		}
		// for rightclick clear
		if(event->button == 3)
		{
			a[data->x][data->y]=0;
			set_elements();
			for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						check_and_mark(i,j);
			is_label_clicked = false;
		}
	}
}

void solve_button(GtkWidget* entry,gpointer data)
{
	get_elements();
	if(solve())
	{
		set_elements();
	}
	else
		open_dialog("Error","No solution for this configuration");
	// clear cells
	coordinate.clear();	
	
}
void next_button(GtkWidget* entry,gpointer data)
{
	get_elements();
	
	if(solve())
	{
		int i = coordinate[0].x;
		int j = coordinate[0].y;
		set_text_single_cell(i,j,"blue");
	}
	else
		open_dialog("Error","No solution for this configuration");
	coordinate.clear();	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			a[i][j] = 0;
		}
		
	}
	
}


bool solve()
{
	// list all unsolved boxes
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if(a[i][j] == 0)
			{
				mypair temp(i,j);
				coordinate.push_back(temp);
			}
		}
	return backtrack(0);	
}

bool backtrack(int index)
{
	if(!coordinate.size())
		return true;
	// try number 1-9 in every unsolved cell
	for (int i = 1; i < 10; i++)
	{
		a[coordinate[index].x][coordinate[index].y] = i; 
		
		// number satisfies condition 
		if(checkall(coordinate[index].x,coordinate[index].y))
		{
			if(coordinate.size() - 1 == index) // return true if it is the last unsolved
				return true;
				
			else if (backtrack(index+1))
				return true; // proceed to the next unsolved
		}
		else
			a[coordinate[index].x][coordinate[index].y] = 0;
	}
	
	// none of them works
	return false;
}
