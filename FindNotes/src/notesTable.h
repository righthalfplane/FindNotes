struct NoteList{
	char *note;
	double frequency;
};

struct NodeGroups{
	struct NoteList *list;
	int listCount;
	char *name;
};

struct NoteList notelist0[]={

{(char *)"A0",27.50},
{(char *)"B0",30.50},
{(char *)"C1",32.70},
{(char *)"D1",36.71},
{(char *)"E1",41.21},
{(char *)"F1",43.65},
{(char *)"G1",49.0},

{(char *)"A1",55.00},
{(char *)"B1",61.74},
{(char *)"C2",65.41},
{(char *)"D2",73.42},
{(char *)"E2",82.41},
{(char *)"F2",87.31},
{(char *)"G2",98.00},

{(char *)"A2",110.00},
{(char *)"B2",123.74},
{(char *)"C3",130.81},
{(char *)"D3",146.83},
{(char *)"E3",164.81},
{(char *)"F3",174.61},
{(char *)"G3",196.00},

{(char *)"A3",220.00},
{(char *)"B3",246.94},
{(char *)"C4",261.63},
{(char *)"D4",293.66},
{(char *)"E4",329.63},
{(char *)"F4",349.23},
{(char *)"G4",392.00},

{(char *)"A4",444.00},
{(char *)"B4",493.88},
{(char *)"C5",523.25},
{(char *)"D5",587.33},
{(char *)"E5",659.26},
{(char *)"F5",698.46},
{(char *)"G5",783.99},
   
{(char *)"A5",880.00},
{(char *)"B5",987.77},
{(char *)"C6",1046.50},
{(char *)"D6",1174.70},
{(char *)"E6",1318.50},
{(char *)"F6",1396.90},
{(char *)"G6",1568.90},

{(char *)"A6",1760.00},
{(char *)"B6",1975.50},
{(char *)"C7",2093.00},
{(char *)"D7",2349.30},
{(char *)"E7",2637.00},
{(char *)"F7",2793.80},
{(char *)"G7",3126.00},

{(char *)"A7",3520.00},
{(char *)"B7",3951.10},
{(char *)"C8",4186.00},

};

struct NoteList notelist1[]={
{(char *)"E2",82.41},
{(char *)"A2",110.00},
{(char *)"D3",146.83},
{(char *)"G3",196.00},
{(char *)"B3",246.94},
{(char *)"E4",329.63},
};


struct NoteList notelist2[]={
{(char *)"C5",523.25},
{(char *)"D5",587.33},
{(char *)"E5",659.26},
{(char *)"F5",698.46},
{(char *)"G5",783.99}, 
{(char *)"A5",880.00},
{(char *)"B5",987.77},
};

struct NodeGroups group[]=
{
	{notelist0,
	sizeof(notelist0)/sizeof(struct NoteList),
	(char *)"Piano"},
	{notelist1,
	sizeof(notelist1)/sizeof(struct NoteList),
	(char *)"Guitar"},
	{notelist2,
	sizeof(notelist2)/sizeof(struct NoteList),
	(char *)"Whistle"},
};

int groupCount=sizeof(group)/sizeof(struct NodeGroups);
