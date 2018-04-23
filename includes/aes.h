#ifndef AES_H
# define AES_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
//#include <dlfcn.h>
#include <fcntl.h>

# define BOOLEAN int

typedef enum		e_bool
{
	true = 1,
	false = 0
}					t_bool;

/*
**	CIPHER PLAIN
*/
typedef struct		s_cipher_plain
{
	char			*cipher;
	char			*plain;
	int				cipher_len;
	int				plain_len;
}					t_cipher_plain;

t_cipher_plain		*get_cipher_plain();
int 				set_plain(t_cipher_plain *cipher_plain, char *plain, int len);
int					set_cipher(t_cipher_plain *cipher_plain, char *cipher, int len);

/*
**	KEY_IV
*/
typedef struct		s_key_iv
{
	char			*key;
	char			*iv;
	int				key_len;
	int				iv_len;
}					t_key_iv;

t_key_iv			*gen_key_iv(char *key, char *iv, int key_len, int iv_len);
void 				free_key_iv(t_key_iv *key_iv);

/*
**	AES
*/
#define AES_SUCCESS	0
#define KEY_SIZE	32
#define IV_SIZE		16
typedef struct		s_aes
{
	BOOLEAN			valid;
	EVP_CIPHER_CTX	*ctx_encryption;
	EVP_CIPHER_CTX	*ctx_decryption;
	t_key_iv		*key_iv;
}					t_aes;

t_key_iv			*get_key_iv(t_aes *aes);
t_aes				*load_aes(void);
void				free_aes(t_aes *aes);
BOOLEAN 			init_encryption(t_aes *aes);
BOOLEAN 			init_decryption(t_aes *aes);

BOOLEAN				encrypt_plain_text(t_aes *aes, t_cipher_plain *cipher_plain);
BOOLEAN				decrypt_cipher_text(t_aes *aes, t_cipher_plain *cipher_plain);

#endif
