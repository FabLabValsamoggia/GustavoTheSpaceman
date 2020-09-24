#ifndef _STR_HARDWARE
#define _STR_HARDWARE
#define MATRIX_PLED         9     // pin per i led (D9)    
#define MATRIX_NLED         16    // numero di led nella matrice
#define BTN1                2     // pin tasto 1
#define BTN2                3     // pin tasto 2
#define BTN3                4     // pin tasto 3
#define BTN4                5     // pin tasto 4 

#define PHOTO1              A7    // pin fotoresistenza 1
#define PHOTO2              A6    // pin fotoresistenza 2
#define PHOTO3              A5    // pin fotoresistenza 3
#define PHOTO4              A4    // pin fotoresistenza 4

typedef struct
{
    bool val     ; /* valore attuale in lettura */
    bool old_val ; /* vecchio valore letto */
}btn_state_t;

typedef struct
{
  int32_t r      ; /* valore rosso da impostare    */
  int32_t g      ; /* valore di verde da impostare */
  int32_t b      ; /* valore di blu da impostare   */
}matrix_rgb_t;

typedef struct
{
  int val        ; /* valore attuale in lettura */
  int old_val    ; /* vecchio valore letto */
}phts_t;
#endif


