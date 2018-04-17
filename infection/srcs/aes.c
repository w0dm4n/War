/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:11:43 by jguyet            #+#    #+#             */
/*   Updated: 2017/03/13 12:15:42 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"

BOOLEAN 		set_plain(t_cipher_plain *cipher_plain, char *plain, int len)
{
	if (!(cipher_plain->plain = (char*)malloc(len)))
		return false;
	cipher_plain->plain_len = len;
	memcpy(cipher_plain->plain, plain, len);
	return true;
}

BOOLEAN			set_cipher(t_cipher_plain *cipher_plain, char *cipher, int len)
{
	if (!(cipher_plain->cipher = (char*)malloc(len)))
		return false;
	cipher_plain->cipher_len = len;
	memcpy(cipher_plain->cipher, cipher, len);
	return true;
}

t_cipher_plain	*get_cipher_plain()
{
	t_cipher_plain *cipher_plain = NULL;

	if (!(cipher_plain = (t_cipher_plain*)malloc(sizeof(struct s_cipher_plain))))
		return (NULL);
	cipher_plain->cipher = NULL;
	cipher_plain->plain = NULL;
	cipher_plain->cipher_len = 0;
	cipher_plain->plain_len = 0;
	return (cipher_plain);
}

t_key_iv			*get_key_iv(t_aes *aes)
{
	const unsigned char key[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	const unsigned char iv[] = { 0x00, 0xfa, 0xda, 0x94, 0x44, 0x55, 0x66, 0x7d, 0xbc, 0x99, 0xbc, 0xcd, 0xcc, 0xef, 0xff, 0x1a };
	return (aes->key_iv = gen_key_iv((char*)&key, (char*)&iv, sizeof(key), sizeof(iv)));
}

t_key_iv			*gen_key_iv(char *key, char *iv, int key_len, int iv_len)
{
	t_key_iv		*key_iv = NULL;

	if (!(key_iv = (t_key_iv*)malloc(sizeof(struct s_key_iv))))
		return (NULL);
	if (!(key_iv->key = (char*)malloc(key_len)))
		return (NULL);
	if (!(key_iv->iv = (char*)malloc(iv_len)))
		return (NULL);
	key_iv->key_len = key_len;
	key_iv->iv_len = iv_len;
	memcpy(key_iv->key, key, key_len);
	memcpy(key_iv->iv, iv, iv_len);
	return key_iv;
}


void 				free_key_iv(t_key_iv *key_iv)
{
	if (key_iv->key)
		free(key_iv->key);
	if (key_iv->iv)
		free(key_iv->iv);
	free(key_iv);
}

int 				init_encryption(t_aes *aes)
{
	return EVP_CIPHER_CTX_ctrl(aes->ctx_encryption, EVP_CTRL_GCM_SET_IVLEN, aes->key_iv->iv_len, NULL) == 1
		&& EVP_EncryptInit_ex(aes->ctx_encryption, NULL, NULL, (unsigned char *)aes->key_iv->key, (unsigned char *)aes->key_iv->iv) == 1;
}

int 				init_decryption(t_aes *aes)
{
	return EVP_CIPHER_CTX_ctrl(aes->ctx_decryption, EVP_CTRL_GCM_SET_IVLEN, aes->key_iv->iv_len, NULL) == 1
		&& EVP_DecryptInit_ex(aes->ctx_decryption, NULL, NULL, (unsigned char *)aes->key_iv->key, (unsigned char *)aes->key_iv->iv) == 1;
}

t_aes				*load_aes()
{
	t_aes		*aes = NULL;

	OpenSSL_add_all_algorithms();
	if (!(aes = (t_aes*)malloc(sizeof(struct s_aes))))
		return (NULL);
	if (!(aes->ctx_encryption = EVP_CIPHER_CTX_new()))
		return (NULL);
	if (!(aes->ctx_decryption = EVP_CIPHER_CTX_new()))
		return (NULL);
	if (!EVP_EncryptInit_ex(aes->ctx_encryption, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (NULL);
	if (!EVP_DecryptInit_ex(aes->ctx_decryption, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (NULL);
	aes->key_iv = NULL;
	aes->valid = false;
	return (aes);
}

void				free_aes(t_aes *aes)
{
	if (aes->key_iv)
		free_key_iv(aes->key_iv);
	EVP_CIPHER_CTX_free(aes->ctx_encryption);
	EVP_CIPHER_CTX_free(aes->ctx_decryption);
	free(aes);
}

BOOLEAN				encrypt_plain_text(t_aes *aes, t_cipher_plain *cipher_plain)
{
	int len = 0;
	if (cipher_plain->cipher != NULL) {
		free(cipher_plain->cipher);
		cipher_plain->cipher = NULL;
	}
	if (!(cipher_plain->cipher = (char*)malloc(cipher_plain->plain_len)))
		return false;
	if (!EVP_EncryptUpdate(aes->ctx_encryption, (unsigned char *)cipher_plain->cipher, &len, (unsigned char *)cipher_plain->plain, cipher_plain->plain_len))
		return false;
	cipher_plain->cipher_len = len;
	return (cipher_plain->cipher_len > 0) ? true : false;
}

BOOLEAN				decrypt_cipher_text(t_aes *aes, t_cipher_plain *cipher_plain)
{
	int len = 0;
	if (cipher_plain->plain != NULL) {
		free(cipher_plain->plain);
		cipher_plain->plain = NULL;
	}
	if ((cipher_plain->plain = (char*)malloc(cipher_plain->cipher_len)) == NULL)
		return false;
	if (!EVP_DecryptUpdate(aes->ctx_decryption, (unsigned char *)cipher_plain->plain, &len, (unsigned char *)cipher_plain->cipher, cipher_plain->cipher_len))
		return false;
	cipher_plain->plain_len = len;
	return (len > 0) ? true : false;
}
