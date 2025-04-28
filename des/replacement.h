#ifndef C_REPLACEMENT_H
#define C_REPLACEMENT_H
unsigned long IP_replacement(unsigned long key, unsigned long* ret, const unsigned char* ip_table);
unsigned long E_replacement(unsigned long key, unsigned long* ret);
unsigned long S_replacement(unsigned long key, unsigned long* ret);
unsigned long S_replacement_single(unsigned long key, unsigned long* ret, int i);
unsigned long PC1_replacement(unsigned long key, unsigned long* ret);
unsigned long PC2_replacement(unsigned long key, unsigned long* ret);
unsigned long P_replacement(unsigned long key, unsigned long* ret);

extern const unsigned char s_box[8][4][16];
#endif
