#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>

const int Fsize=50;
int no_edges;
int no_nodes;
int i;
const int cases = 30;
//this function finds binary connective of a given string;
//return -1 if not found
int getConnectiveIndex(char *g){
	int i;
	int counter = 0;
	int length = strlen(g);
	for(i = 0 ; i < length ; i++){
		if(g[i] == '('){
			counter++;
		}
		else if(g[i] == ')'){
			counter--;
		}
		else if(g[i]=='>'|| g[i]=='^'|| g[i]=='v'){
			if (counter == 1){
				return i;
			}
		}
	}
	return -1;
}

char *partone(char *g){
	int i;
	int bin = getConnectiveIndex(g);
	char *before = (char *)malloc(Fsize);
	for(i = 1 ; i < bin ; i++){
		before[i-1] = g[i];
	}
	before[i+1] = '\0';
	return before;
}

char bin(char *g){
	int bin = getConnectiveIndex(g);
	return g[bin];
}

char *parttwo(char *g){
	int i;
	int bin = getConnectiveIndex(g);
	int length = strlen(g);
	char *after = (char *)malloc(Fsize);
	for(i = (bin+1) ; i < (length-1) ; i++){
		after[i-(bin+1)] = g[i];
	}
	after[i+1] = '\0';
	return after;
}

int is_edge(int firstnode, int secondnode, int edges[no_edges][2]){
	int i = 0;
	for(i = 0 ; i < no_edges ; i++){
		if(firstnode == edges[i][0] && secondnode == edges[i][1]){
			return 1;
		}
	}
	return 0;
}

char getVariable(char var, int V[3]){
	switch(var){
		case('x'):return V[0];
				  break;
		case('y'):return V[1];
		          break;
		case('z'):return V[2];
		          break;
		default: return -1;
	}
}

int parse(char *g)
/* returns 0 for non-formulas, 1 for atoms, 2 for negations, 3 for binary connective fmlas, 4 for existential and 5 for universal formulas.*/
{
	//0:check for number of brackets
	int counter = 0;
	int i;
	int length = strlen(g);
	for(i = 0 ; i < length ; i++){
		if(g[i] == '('){
			counter++;
		}
		else if(g[i] == ')'){
			counter--;
		}
	}

	// case where it is an atomic formula
	if(g[0] == 'X'){
		if(g[1] == '['){
			if(g[2] == 'x' || g[2] == 'y' || g[2] == 'z'){
				if(g[3] == 'y' || g[3] == 'z' || g[3] == 'x'){
					if(g[4] == ']'){
						if(g[5] == '\0'){
							return 1;
						}
					}
				}
			}
		}

	}
	//2:case of negation
	else if(g[0] == '-'){
		if(g[1] != '\0'){
			if(parse(g+1)>0){
				return 2;
			}
		}

	}
	//4:case of Existential formula
	else if(g[0] == 'E'){
		if(g[1] == 'x' || g[1] == 'y' || g[1] == 'z'){
				if(g[2] != '\0' && parse(g+2)>0){
				return 4;
			}

		}
			
			
	}
	//5:case of Universal formula
	else if(g[0] == 'A'){
		if(g[1] == 'x' || g[1] == 'y' || g[1] == 'z'){
				if(g[2] != '\0' && parse(g+2)>0){
					return 5;
				}		
		}
	}
	//3:case of Binary connective formula
	else if(g[0] == '('){
		// puts("in binary connective");
		if(counter == 0){
			// puts("counter check passed");
			// printf("part one is: %s\n", partone(g));
			// printf("part two is: %s\n", parttwo(g));
			// printf("part one parse result: %d\n", parse(partone(g)));
			// printf("part two parse result: %d\n", parse(parttwo(g)));
			if(parse(partone(g)) > 0 && parse(parttwo(g)) > 0){
				// puts("is binary connective");
				return 3;
			}

		}
		
	}
		
	//A fail case to catch all the failures   
	return 0;
}

int eval(char *nm, int edges[no_edges][2], int size, int V[3])
/*this method takes a formula, the list of edges of a graph, the number of vertices and a variable assignment.  It then evaluates the formula and returns 1 or 0 as appropriate.  */
{
	int i, first, second;
	char bin = nm[getConnectiveIndex(nm)];
	char *before = partone(nm);
	char *after = parttwo(nm);
	switch(parse(nm))
	{   
		case(1)://Atomic formula
		first = getVariable(nm[2],V);
		second = getVariable(nm[3],V);
		if(is_edge(first,second,edges) == 1){
			return 1;
		}
		else return 0;
		break;

		case(2)://negation formula
		if(eval(nm+1,edges,size,V) == 0){
			return 1;
		}
		else return 0;
		break;

		case(3)://Binary connective formula
		switch(bin){
			case 'v' :if(eval(before,edges,size,V) == 1 || eval(after,edges,size,V) == 1){
				return 1;
			}
			else return 0;
			case '^' :if(eval(before,edges,size,V) == 1 && eval(after,edges,size,V) == 1){
				return 1;
			} 
			else return 0;
			case '>' :if(eval(before,edges,size,V) == 1){
				if(eval(after,edges,size,V) == 1){
					// puts("second part passed");
					return 1;
				}
				else return 0;
			}
			else return 1;

		}
		break;

		case(4)://Existential formula
		for(i = 0 ; i < no_nodes ; i++){
			switch(nm[1]){
				case 'x': V[0] = i; break;
				case 'y': V[1] = i; break;
				case 'z': V[2] = i; break;
				default: return -1;
			}
			if(eval(nm+2,edges,size,V) == 1){
				return 1;
			}
		}
		return 0;
		break;

		case(5)://Universal formula
		for(i = 0 ; i < no_nodes ; i++){
			switch(nm[1]){
				case 'x': V[0] = i; break;
				case 'y': V[1] = i; break;
				case 'z': V[2] = i; break;
				default: return -1;
			}
			if(eval(nm+2,edges,size,V) == 0){
				return 0;
			}

		}
		return 1;
		break;

		default: return -1;

	}

}

int main(void)
{
  char *name=malloc(Fsize); /*create space for the formula*/
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
 if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s %d %d",name,&no_nodes,&no_edges);/*read formula, number of nodes, number of edges*/
      int edges[no_edges][2];  /*Store edges in 2D array*/
      for(i=0;i<no_edges;i++)	 fscanf(fp, "%d%d", &edges[i][0], &edges[i][1]);/*read all the edges*/

      /*Assign variables x, y, z to nodes */
      int W[3];
      /*Store variable values in array
	value of x in V[0]
	value of y in V[1]
	value of z in V[2] */
      for(i=0;i<3;i++)  fscanf(fp, "%d", &W[i]);
      int p=parse(name); 
      switch(p)
	{case 0:fprintf(fpout,"%s is not a formula.  ", name);break;
	case 1: fprintf(fpout,"%s is an atomic formula.  ",name);break;
	case 2: fprintf(fpout,"%s is a negated formula.  ",name);break;
	case 3: fprintf(fpout,"%s is a binary connective formula.  ", name);break;
	case 4: fprintf(fpout,"%s is an existential formula.  ",name);break;
	case 5: fprintf(fpout,"%s is a universal formula.  ",name);break;
	default: fprintf(fpout,"%s is not a formula.  ",name);break;
	}
  
      /*Now check if formula is true in the graph with given variable assignment. */
      if (eval(name, edges, no_nodes,  W)==1)	fprintf(fpout,"The formula %s is true in G under W\n", name); 
      else fprintf(fpout,"The formula %s is false in G under W\n", name);
    }
 
  fclose(fp);
  fclose(fpout);
  free(name);
  return(1);
}
