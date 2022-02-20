#ifndef __EXTLIB_H__
#define __EXTLIB_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

typedef signed char s8;
typedef unsigned char u8;
typedef signed short int s16;
typedef unsigned short int u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long int s64;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;
typedef uintptr_t uPtr;
typedef intptr_t sPtr;
typedef u32 void32;
typedef time_t Time;

typedef struct {
	f32 h;
	f32 s;
	f32 l;
} HSL8;

typedef struct {
	f32 h;
	f32 s;
	f32 l;
	u8  alpha;
} HSLA8;

typedef struct {
	union {
		struct {
			u8 r;
			u8 g;
			u8 b;
		};
		u8 c[3];
	};
} RGB8;

typedef struct {
	union {
		struct {
			u8 r;
			u8 g;
			u8 b;
			u8 a;
		};
		u8 c[4];
	};
} RGBA8;

typedef struct {
	f32 r;
	f32 g;
	f32 b;
} RGB32;

typedef struct {
	f32 r;
	f32 g;
	f32 b;
	f32 a;
} RGBA32;

typedef enum {
	PSL_DEBUG = -1,
	PSL_NONE,
	PSL_NO_INFO,
	PSL_NO_WARNING,
	PSL_NO_ERROR,
} PrintfSuppressLevel;

typedef enum {
	SWAP_U8  = 1,
	SWAP_U16 = 2,
	SWAP_U32 = 4,
	SWAP_U64 = 8,
	SWAP_F80 = 10
} SwapSize;

typedef union {
	void* p;
	u8*   u8;
	u16*  u16;
	u32*  u32;
	u64*  u64;
	s8*   s8;
	s16*  s16;
	s32*  s32;
	s64*  s64;
	f32*  f32;
	f64*  f64;
} PointerCast;

typedef struct Node {
	struct Node* prev;
	struct Node* next;
} Node;

typedef enum {
	MEM_FILENAME = 1 << 16,
	MEM_CRC32    = 1 << 17,
	MEM_ALIGN    = 1 << 18,
	MEM_REALLOC  = 1 << 19,
	
	MEM_CLEAR    = 1 << 30,
	MEM_END      = 1 << 31,
} MemInit;

typedef struct MemFile {
	union {
		void* data;
		PointerCast cast;
		char* str;
	};
	u32 memSize;
	u32 dataSize;
	u32 seekPoint;
	struct {
		Time  age;
		char* name;
		u32   crc32;
	} info;
	struct {
		u32 align   : 8;
		u32 realloc : 1;
		u32 getCrc  : 1;
		u32 getName : 1;
		u64 initKey;
	} param;
} MemFile;

typedef struct ItemList {
	char*  buffer;
	u32    writePoint;
	char** item;
	u32    num;
} ItemList;

typedef enum {
	DIR__MAKE_ON_ENTER = (1) << 0,
} DirParam;

extern u8 gPrintfProgressing;

void SetSegment(const u8 id, void* segment);
void* SegmentedToVirtual(const u8 id, void32 ptr);
void32 VirtualToSegmented(const u8 id, void* ptr);

void* Tmp_Alloc(u32 size);
char* Tmp_String(char* str);
char* Tmp_Printf(char* fmt, ...);

void Log(const char* fmt, ...);
void LogPrint();

void Time_Start(void);
f32 Time_Get(void);

typedef enum {
	SORT_NO        = 0,
	SORT_NUMERICAL = 1,
	IS_FILE        = 0,
	IS_DIR         = 1,
} DirEnum;

void Dir_SetParam(DirParam w);
void Dir_UnsetParam(DirParam w);
void Dir_Set(char* path, ...);
void Dir_Enter(char* ent, ...);
void Dir_Leave(void);
void Dir_Make(char* dir, ...);
void Dir_MakeCurrent(void);
char* Dir_Current(void);
char* Dir_File(char* fmt, ...);
Time Dir_Stat(char* item);
void Dir_ItemList(ItemList* itemList, bool isPath);
void Dir_ItemList_Recursive(ItemList* target, char* keyword);
void Dir_ItemList_Not(ItemList* itemList, bool isPath, char* not);
void Dir_ItemList_Keyword(ItemList* itemList, char* ext);

void MakeDir(const char* dir, ...);
Time Stat(char* item);
char* CurWorkDir(void);

void ItemList_NumericalSort(ItemList* list);
char* Dir_GetWildcard(char* x);

void printf_SetSuppressLevel(PrintfSuppressLevel lvl);
void printf_SetPrefix(char* fmt);
void printf_SetPrintfTypes(const char* d, const char* w, const char* e, const char* i);
void printf_toolinfo(const char* toolname, const char* fmt, ...);
void printf_debug(const char* fmt, ...);
void printf_debug_align(const char* info, const char* fmt, ...);
void printf_warning(const char* fmt, ...);
void printf_warning_align(const char* info, const char* fmt, ...);
void printf_error(const char* fmt, ...);
void printf_error_align(const char* info, const char* fmt, ...);
void printf_info(const char* fmt, ...);
void printf_info_align(const char* info, const char* fmt, ...);
void printf_progress(const char* info, u32 a, u32 b);
s32 printf_get_answer(void);
void printf_WinFix();

void* MemMem(const void* haystack, size_t haystackSize, const void* needle, size_t needleSize);
void* MemMemCase(const void* haystack, size_t haystackSize, const void* needle, size_t needleSize);
void* MemMem16(const void* haystack, size_t haySize, const void* needle, size_t needleSize);
void* MemMemU16(void* haystack, size_t haySize, const void* needle, size_t needleSize);
void* MemMemU32(void* haystack, size_t haySize, const void* needle, size_t needleSize);
void* MemMemU64(void* haystack, size_t haySize, const void* needle, size_t needleSize);
void* Malloc(void* data, s32 size);
void* Calloc(void* data, s32 size);
void* Realloc(void* data, s32 size);
void* Free(void* data);
s32 Touch(char* file);
void ByteSwap(void* src, s32 size);
s32 ParseArgs(char* argv[], char* arg, u32* parArg);
u32 Crc32(u8* s, u32 n);

void Color_ToHSL(HSL8* hsl, RGB8* rgb);
void Color_ToRGB(RGB8* rgb, HSL8* hsl);

void* File_Load(void* destSize, char* filepath);
void File_Save(char* filepath, void* src, s32 size);
void* File_Load_ReqExt(void* size, char* filepath, const char* ext);
void File_Save_ReqExt(char* filepath, void* src, s32 size, const char* ext);

MemFile MemFile_Initialize();
void MemFile_Params(MemFile* memFile, ...);
void MemFile_Malloc(MemFile* memFile, u32 size);
void MemFile_Realloc(MemFile* memFile, u32 size);
void MemFile_Rewind(MemFile* memFile);
s32 MemFile_Write(MemFile* dest, void* src, u32 size);
s32 MemFile_Append(MemFile* dest, MemFile* src);
void MemFile_Align(MemFile* src, u32 align);
s32 MemFile_Printf(MemFile* dest, const char* fmt, ...);
s32 MemFile_Read(MemFile* src, void* dest, u32 size);
void MemFile_Seek(MemFile* src, u32 seek);
s32 MemFile_LoadFile(MemFile* memFile, char* filepath);
s32 MemFile_LoadFile_String(MemFile* memFile, char* filepath);
s32 MemFile_SaveFile(MemFile* memFile, char* filepath);
s32 MemFile_SaveFile_String(MemFile* memFile, char* filepath);
s32 MemFile_LoadFile_ReqExt(MemFile* memFile, char* filepath, const char* ext);
s32 MemFile_SaveFile_ReqExt(MemFile* memFile, char* filepath, s32 size, const char* ext);
void MemFile_Free(MemFile* memFile);
void MemFile_Reset(MemFile* memFile);
void MemFile_Clear(MemFile* memFile);

#define StrStr(src, comp)     MemMem(src, strlen(src), comp, strlen(comp))
#define StrStrCase(src, comp) MemMemCase(src, strlen(src), comp, strlen(comp))
u32 String_GetHexInt(char* string);
s32 String_GetInt(char* string);
f32 String_GetFloat(char* string);
s32 String_GetLineCount(char* str);
s32 String_CaseComp(char* a, char* b, u32 compSize);
char* String_Line(char* str, s32 line);
char* String_Word(char* str, s32 word);
char* String_GetLine(const char* str, s32 line);
char* String_GetWord(const char* str, s32 word);
void String_CaseToLow(char* s, s32 i);
void String_CaseToUp(char* s, s32 i);
char* String_GetPath(const char* src);
char* String_GetBasename(const char* src);
char* String_GetFilename(const char* src);
s32 String_GetPathNum(const char* src);
char* String_GetFolder(const char* src, s32 num);
void String_Insert(char* point, char* insert);
void String_Remove(char* point, s32 amount);
s32 String_Replace(char* src, char* word, char* replacement);
void String_SwapExtension(char* dest, char* src, const char* ext);
char* String_GetSpacedArg(char* argv[], s32 cur);

void Config_SuppressNext(void);
char* Config_Get(MemFile* memFile, char* name);
s32 Config_GetBool(MemFile* memFile, char* boolName);
s32 Config_GetOption(MemFile* memFile, char* stringName, char* strList[]);
s32 Config_GetInt(MemFile* memFile, char* intName);
char* Config_GetString(MemFile* memFile, char* stringName);
f32 Config_GetFloat(MemFile* memFile, char* floatName);

#define str2cmp(a, b) strncmp(a, b, strlen(b))

#define Node_Add(head, node) { \
		typeof(node) lastNode = head; \
		if (lastNode == NULL) { \
			head = node; \
		} else { \
			while (lastNode->next) { \
				lastNode = lastNode->next; \
			} \
			lastNode->next = node; \
			node->prev = lastNode; \
		} \
}

#define Node_Kill(head, node) { \
		if (node->next) { \
			node->next->prev = node->prev; \
		} \
		if (node->prev) { \
			node->prev->next = node->next; \
		} else { \
			head = node->next; \
		} \
		free(node); \
		node = NULL; \
}

#define Node_Remove(head, node) { \
		if (node->next) { \
			node->next->prev = node->prev; \
		} \
		if (node->prev) { \
			node->prev->next = node->next; \
		} else { \
			head = node->next; \
		} \
		node = NULL; \
}

#define Swap(a, b) { \
		typeof(a) y = a; \
		a = b; \
		b = y; \
}

#define WrapF(x, min, max) ({ \
		typeof(x) r = (x); \
		typeof(x) range = (max) - (min) + 1; \
		if (r < (min)) { \
			r += range * (((min) - r) / range + 1); \
		} \
		(min) + fmod((r - (min)), range); \
	})

#define Wrap(x, min, max) ({ \
		typeof(x) r = (x); \
		typeof(x) range = (max) - (min) + 1; \
		if (r < (min)) { \
			r += range * (((min) - r) / range + 1); \
		} \
		(min) + (r - (min)) % range; \
	})

// Checks endianess with tst & tstP
#define ReadBE(in) ({ \
		typeof(in) out; \
		s32 tst = 1; \
		u8* tstP = (u8*)&tst; \
		if (tstP[0] != 0) { \
			s32 size = sizeof(in); \
			if (size == 2) { \
				out = __builtin_bswap16(in); \
			} else if (size == 4) { \
				out = __builtin_bswap32(in); \
			} else if (size == 8) { \
				out = __builtin_bswap64(in); \
			} else { \
				out = in; \
			} \
		} else { \
			out = in; \
		} \
		out; \
	} \
)

#define WriteBE(dest, set) { \
		typeof(dest) get = set; \
		dest = ReadBE(get); \
}

#define SwapBE(in) WriteBE(in, in)

#define Decr(x) (x -= (x > 0) ? 1 : 0)
#define Incr(x) (x += (x < 0) ? 1 : 0)

extern PrintfSuppressLevel gPrintfSuppress;

#define PRNT_DGRY "\e[90;2m"
#define PRNT_DRED "\e[91;2m"
#define PRNT_GRAY "\e[0;90m"
#define PRNT_REDD "\e[0;91m"
#define PRNT_GREN "\e[0;92m"
#define PRNT_YELW "\e[0;93m"
#define PRNT_BLUE "\e[0;94m"
#define PRNT_PRPL "\e[0;95m"
#define PRNT_CYAN "\e[0;96m"
#define PRNT_RSET "\e[m"
#define PRNT_NL   "\n"
#define PRNT_RNL  PRNT_RSET PRNT_NL
#define PRNT_TODO "\e[91;2m" "TODO"

#define MAX(a, b)            ((a) > (b) ? (a) : (b))
#define MIN(a, b)            ((a) < (b) ? (a) : (b))
#define ABS_MAX(a, b)        (ABS(a) > ABS(b) ? (a) : (b))
#define ABS_MIN(a, b)        (ABS(a) < ABS(b) ? (a) : (b))
#define ARRAY_COUNT(arr)     (s32)(sizeof(arr) / sizeof(arr[0]))
#define ABS(val)             ((val) < 0 ? -(val) : (val))
#define CLAMP(val, min, max) ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))
#define CLAMP_MIN(val, min)  ((val) < (min) ? (min) : (val))
#define CLAMP_MAX(val, max)  ((val) > (max) ? (max) : (val))
#define ArrayCount(arr)      (u32)(sizeof(arr) / sizeof(arr[0]))
#define Lerp(x, min, max)    ((min) + ((max) - (min)) * (x))

#define BinToMb(x)        ((f32)(x) / (f32)0x100000)
#define BinToKb(x)        ((f32)(x) / (f32)0x400)
#define MbToBin(x)        (u32)(0x100000 * (x))
#define KbToBin(x)        (u32)(0x400 * (x))
#define Align(var, align) ((((var) % (align)) != 0) ? (var) + (align) - ((var) % (align)) : (var))

// #define strcpy(dst, src)   strcpy(dst, src)
// #define strcat(dst, src)  strcat(dst, src)
#define String_SMerge(dst, ...) sprintf(dst + strlen(dst), __VA_ARGS__);
#define String_Generate(string) strdup(string)

#define Config_WriteTitle(title) MemFile_Printf( \
		config, \
		title \
)

#define Config_WriteTitle_Str(title) MemFile_Printf( \
		config, \
		"# %s\n", \
		title \
)

#define Config_WriteVar(com1, name, defval, com2) MemFile_Printf( \
		config, \
		com1 \
		"%-15s = %-10s # %s\n\n", \
		name, \
		# defval, \
		com2 \
)

#define Config_WriteVar_Hex(name, defval) MemFile_Printf( \
		config, \
		"%-15s = 0x%X\n", \
		name, \
		defval \
)

#define Config_WriteVar_Int(name, defval) MemFile_Printf( \
		config, \
		"%-15s = %d\n", \
		name, \
		defval \
)

#define Config_WriteVar_Flo(name, defval) MemFile_Printf( \
		config, \
		"%-15s = %f\n", \
		name, \
		defval \
)

#define Config_WriteVar_Str(name, defval) MemFile_Printf( \
		config, \
		"%-15s = %s\n", \
		name, \
		defval \
)

#define Config_SPrintf(...) MemFile_Printf( \
		config, \
		__VA_ARGS__ \
)

#ifndef NDEBUG
	#define printf_debugExt(...) if (gPrintfSuppress <= PSL_DEBUG) { \
			if (gPrintfProgressing) { printf("\n"); gPrintfProgressing = 0; } \
			printf(PRNT_PRPL "[X]: " PRNT_CYAN "%-16s " PRNT_REDD "%s" PRNT_GRAY ": " PRNT_YELW "%d" PRNT_RSET "\n", __FUNCTION__, __FILE__, __LINE__); \
			printf_debug(__VA_ARGS__); \
	}
	
	#define printf_debugExt_align(title, ...) if (gPrintfSuppress <= PSL_DEBUG) { \
			if (gPrintfProgressing) { printf("\n"); gPrintfProgressing = 0; } \
			printf(PRNT_PRPL "[X]: " PRNT_CYAN "%-16s " PRNT_REDD "%s" PRNT_GRAY ": " PRNT_YELW "%d" PRNT_RSET "\n", __FUNCTION__, __FILE__, __LINE__); \
			printf_debug_align(title, __VA_ARGS__); \
	}
	
	#define Assert(exp) if (!(exp)) { \
			if (gPrintfProgressing) { printf("\n"); gPrintfProgressing = 0; } \
			printf(PRNT_PRPL "[X]: " PRNT_CYAN "%-16s " PRNT_REDD "%s" PRNT_GRAY ": " PRNT_YELW "%d" PRNT_RSET "\n", __FUNCTION__, __FILE__, __LINE__); \
			printf_debug(PRNT_YELW "Assert(\a " PRNT_RSET # exp PRNT_YELW " );"); \
			exit(EXIT_FAILURE); \
	}
	
    #ifndef __EXTLIB_C__
		
		#define Malloc(data, size) Malloc(data, size); \
			printf_debugExt_align("Malloc", "0x%X", size);
		
		#define Calloc(data, size) Calloc(data, size); \
			printf_debugExt_align("Calloc", "0x%X", size);
		
    #endif
#else
	#define printf_debugExt(...)       if (0) {}
	#define printf_debugExt_align(...) if (0) {}
	#define Assert(exp)                if (0) {}
    #ifndef __EXTLIB_C__
		#define printf_debug(...)       if (0) {}
		#define printf_debug_align(...) if (0) {}
    #endif
#endif

#define Main(y1, y2) main(y1, y2)

#define XARG(arg) ParseArgs(argv, arg, &parArg)

#define AttPacked __attribute__ ((packed))
#define AttAligned(x) __attribute__((aligned(x)))

#define ParseArg(xarg)         ParseArgs(argv, xarg, &parArg)
#define EXT_INFO_TITLE(xtitle) PRNT_YELW xtitle PRNT_RNL
#define EXT_INFO(A, indent, B) PRNT_GRAY "[>] " PRNT_RSET A "\r\033[" #indent "C" PRNT_GRAY "# " B PRNT_NL

#define renamer_remove(old, new) \
	if (rename(old, new)) { \
		if (remove(new)) { \
			printf_error_align( \
				"Delete failed", \
				"[%s]", \
				new \
			); \
		} \
		if (rename(old, new)) { \
			printf_error_align( \
				"Rename failed", \
				"[%s] -> [%s]", \
				old, \
				new \
			); \
		} \
	}

#endif /* __EXTLIB_H__ */
