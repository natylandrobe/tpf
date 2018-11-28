
struct lat_lon_y_aux
{
	double lat;
	double lon;
	cal_t aux_cal;
};
struct fecha {
        int dia;
        int mes;
        int anio;
        int hora;
        int minutos;
        float segundos;
};


struct GGA{
        char tipo[TIPO_LEN];
        struct fecha f;
        struct lat_lon_y_aux pos;
        double ele;
        double hdop;
        double separacion;
        cal_t calidad;
        long int cantSat;
};

struct RMC{
        char tipo[TIPO_LEN];
        struct fecha f;
        char status;
        struct lat_lon_y_aux pos;
};

struct ZDA{
        char tipo[TIPO_LEN];
        struct fecha f;
        int time_zone;
        int dif_tmzone;
}; 

bool cargar_struct_rmc(char *s,struct RMC *Rmc,struct fecha *date,struct lat_lon_y_aux *pos){

        char *str, *check;
        char status;
        double stat;
        char *tokens[CANT_TOKEN_RMC];
        double lat, lon;
        double horario;
       
     //   if(!s || !newS){
       //         return false;
        //}

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }
        
        horario= strtod(tokens[INDEX_HORARIO_RMC], &check);

        if(*check != END_STR){
                return false;
        }

        stat=strtod(tokens[INDEX_STATUS_RMC], &check);
        
        if(*check != END_STR){
                //esto es solo una prueba de que esto se cumple
                printf("%s\n","ERROR");
                //no se porque si desmarco esto retorna false
              //return false;
        }
        if (stat==65.00)
                status='A';
        if(stat==86.00)
                status='V';


        lat = convertirLat(tokens[INDEX_LAT_RMC], tokens[INDEX_LAT_CARD_RMC]);
        lon = convertirLon(tokens[INDEX_LON_RMC], tokens[INDEX_LON_CARD_RMC]);

        if(!checkMembersrmc(lat, lon)){
                return false;
        }
                
                Rmc->f.hora= horario/10000;
        Rmc->f.minutos = ((int)horario%10000)/100;
        //falta ver los decimales de segundos
        Rmc->f.segundos = ((int)horario%100);
        *date= Rmc->f;
        Rmc->pos.lat = lat;
        Rmc->pos.lon= lon;
        if(pos->aux_cal == invalido){
                *pos=Rmc->pos;
        }
        Rmc->status= status;
        
        return true;
}

bool cargar_struct_gga(char *s,struct GGA *Gga,struct fecha *date,struct lat_lon_y_aux *pos){


        char *str, *check;
        char *tokens[CANT_TOKEN];
        double lat, lon, hdop, ele, separacion;
        long int cal, cant;
        double horario;
        cal_t calidad;

     //   if(!s || !newS){
       //         return false;
        //}

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }

        horario= strtod(tokens[INDEX_HORARIO_GGA], &check);

        if(*check != END_STR){
               return false;
        }

        
        cant = strtol(tokens[INDEX_CANT], &check, 10);

        if(*check != END_STR){
                return false;
        }

        hdop = strtod(tokens[INDEX_HDOP], &check);

        if(*check != END_STR){
                return false;
        }

        ele = strtod(tokens[INDEX_ELE], &check);

        if(*check != END_STR){
                return false;
        }

        separacion= strtod(tokens[INDEX_SEP], &check);

        if(*check != END_STR){
                return false;
        }

        cal = strtol(tokens[INDEX_CAL], &check, 10);

        if(*check != END_STR){
                return false;
        }

        calidad = convertirCal(cal);
        lat = convertirLat(tokens[INDEX_LAT], tokens[INDEX_LAT_CARD]);
        lon = convertirLon(tokens[INDEX_LON], tokens[INDEX_LON_CARD]);

        if(!checkMembers(lat, lon, calidad, cant)){
                return false;
        }

        //Gga->f= date;


        Gga->f.hora= horario/10000;
        Gga->f.minutos = ((int)horario%10000)/100;
        //falta ver los decimales de segundos
        Gga->f.segundos = ((int)horario%100);
        *date=Gga->f;
        Gga->pos.lat = lat;
        Gga->pos.lon = lon;
        Gga->pos.aux_cal=Gga->calidad = calidad;
        *pos=Gga->pos;
        Gga->cantSat = cant;
        Gga->hdop = hdop;
        Gga->ele = ele;
        Gga->separacion = separacion;
        
        return true;
}






