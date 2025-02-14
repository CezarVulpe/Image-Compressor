for (int i = 1; i <= graph->n; i++)
        distance[i] = INT_MAX;

    distance[1] = 0;
    int i = 1;
    int j;
    while (i <= graph->n) {
        int min_distance = INT_MAX;
        j = 1;
        while (j <= graph->n) {
            if (!is_visited[j] && distance[j] < min_distance) {
                min_distance = distance[j];
                current_vertex = j;
            }
            j++;
        }

        is_visited[current_vertex] = 1;
        AArc current_arc = graph->x[current_vertex];
        while (current_arc) {
            if (!is_visited[current_arc->d] && distance[current_arc->d] > distance[current_vertex] + current_arc->c) {
                distance[current_arc->d] = distance[current_vertex] + current_arc->c;
            }
            current_arc = current_arc->urm;
        }
        i++;
    }
    
    
    
    
    
    if(noduri[j].cost<costmin)
                {
                    costmin=noduri[j].cost;
                    urmator=j;
                }
                
                
                
                
                
                
                // score=noduri[j].cost*1.0;
                // score=score/v->vec[j].adancime;
                score=(float)(noduri[j].cost/v->vec[j].adancime);
                if(score<scoreminim)
                {
                    urmator=j;
                    scoreminim=score;
                }
                
                
                
                
                for (int j = 1; j <= g->n; j++)
        {
            if (visited[j] == 0)
            {
                if(noduri[j].cost!=INT_MAX)
                {
                aux[j].c;
                score=v->vec[curent]*1.0;
                score=score/v->vec[j].adancime;
                // score=(float)(noduri[j].cost/v->vec[j].adancime);
                if(score<scoreminim)
                {
                    urmator=j;
                    scoreminim=score;
                }
                }
                
            }
        }
        
        
        
        
        p = g->x + curent;
        aux = *p;
        while (aux != NULL)
        {
            if (visited[aux->d] == 0)
            {
                score=aux->c*1.0;
                score=score/v->vec[aux->d].adancime;
                // score=(float)(noduri[j].cost/v->vec[j].adancime);
                if(score<scoreminim)
                {
                    urmator=aux->d;
                    scoreminim=score;
                }
            }
            aux = aux->urm;
        }
