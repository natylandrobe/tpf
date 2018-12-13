#ifndef UBX_H
#define UBX_H

#include "lista.h"

#define LARGO_CK_SZ 2
#define SB_1 0xB5
#define SB_2 0x62
#define NAV_CL 0x01
#define TIM_CL 0x0D
#define TIM_TOS_ID 0x12
#define PVT_ID 0x07
#define POSLLH_ID 0x02 
#define PVT_L 92
#define TIM_TOS_L 56
#define POSLLH_L 28
#define PVT_BUFFSZ 96
#define TIM_TOS_BUFFSZ 60
#define POSLLH_BUFFSZ 32
#define CLASS_IND 0
#define ID_IND 1
#define LARGO_MSB_IND 3
#define LARGO_LSB_IND 2

/*defines para calc_largo*/
#define MSB_IND 1
#define LSB_IND 0
#define SHIFT_1B 8

/*defines para ubx_cksum*/
#define POS_CK -4
#define CKA_IND 0
#define CKB_IND 1

/*defines para calc_fecha*/
#define P_ANIO_MBIND 9
#define P_ANIO_LBIND 8
#define PVT_MOIND 10
#define PVT_DIND 11
#define PVT_HIND 12
#define PVT_MININD 13
#define PVT_SEGIND 14
#define T_ANIO_MBIND 13
#define T_ANIO_LBIND 12
#define TT_MOIND 14
#define TT_DIND 15
#define TT_HIND 16
#define TT_MININD 17
#define TT_SEGIND 18

/*defines para cargar_pvt */
#define VALID_IND 15
#define FLAG_IND 25
#define MASK_FIX 0x01
#define NUM_IND 27
#define PDOP_MSB 80
#define PDOP_LSB 81

/*defines cargar_pos*/
#define SHIFT_2B 16
#define SHIFT_3B 24
#define PVTLON_B0 28
#define PVTLON_B1 29 
#define PVTLON_B2 30
#define PVTLON_B3 31
#define PVTLAT_B0 32
#define PVTLAT_B1 33 
#define PVTLAT_B2 34
#define PVTLAT_B3 35
#define PVTH_B0 40
#define PVTH_B1 41 
#define PVTH_B2 42
#define PVTH_B3 43
#define POSLON_B0 8
#define POSLON_B1 9 
#define POSLON_B2 10
#define POSLON_B3 11
#define POSLAT_B0 12
#define POSLAT_B1 13 
#define POSLAT_B2 14
#define POSLAT_B3 15
#define POSH_B0 20
#define POSH_B1 21 
#define POSH_B2 22
#define POSH_B3 23
#define SCALING_POS 0.0000001
#define SCALING_PDOP 0.01

/*defines cargar_precision */
#define HACC_B0 27
#define HACC_B1 26
#define HACC_B2 25
#define HACC_B3 24
#define VACC_B0 31
#define VACC_B1 30
#define VACC_B2 29 
#define VACC_B3 28

/*defines cargar timtos */
#define VERSION_IND 4
#define GNSS_IND 5



unsigned int calc_largo(unsigned char info[]);
ubxst_t procesar_ubx(FILE *fin, struct fecha *fecha, lista_t *lista, size_t *index, status_t (*add_nodo)(void *, lista_t *, sent_t), FILE *flog, const struct args *arg);
ubxst_t procesar_standard(struct fecha *fecha, lista_t *lista, size_t *index, status_t (*add_nodo)(void *, lista_t *, sent_t), FILE *flog);
ubxst_t ubx_cksum(const unsigned char *ckBuff, int n, FILE *fin);
ubxst_t calc_fecha(const unsigned char *buff, struct fecha *fecha, unsigned char id);
ubxst_t cargar_fecha(const void *dato, struct fecha *funi, unsigned char id, const unsigned char *buff, ubxst_t (*proc_fecha)(const unsigned char *, struct fecha *, unsigned char));
ubxst_t cargar_precision(struct s_POSLLH *dato, const unsigned char *buff);
ubxst_t cargar_pos(const void *dato, unsigned char id, const unsigned char *buff);
ubxst_t cargar_sPVT(struct s_PVT * dato, struct fecha *funi, const unsigned char *buff);
ubxst_t cargar_sPOSLLH(struct s_POSLLH *dato, struct fecha *funi, const unsigned char *buff);
ubxst_t cargar_sTIMTOS(struct s_TIM_TOS *dato, struct fecha *funi, const unsigned char *buff);


#endif