#include "HermosauhuLib.h"

/* 👺 PRINTF 👺 */
PrintfSuppressLevel gPrintfSuppress = 0;
char* sPrintfPrefix = "👺🚬";

void printf_SetSuppressLevel(PrintfSuppressLevel lvl) {
	gPrintfSuppress = lvl;
}

void printf_SetPrefix(char* fmt) {
	sPrintfPrefix = fmt;
}

void printf_toolinfo(const char* toolname, const char* fmt, ...) {
	if (gPrintfSuppress >= PSL_NO_INFO)
		return;
	va_list args;
	
	// [0;36m%s\e[m
	va_start(args, fmt);
	printf(
		"\e[90;2m"
		"╔══════════════════════════════════╗\n"
		"║                                  ║\n"
	);
	printf("\033[1A" "\033[3C");
	printf("\e[0;96m%s\e[90;2m", toolname);
	printf(
		"\n"
		"╚══════════════════════════════════╝\e[m\n"
	);
	vprintf(
		fmt,
		args
	);
	va_end(args);
}

void printf_debug(const char* fmt, ...) {
	if (gPrintfSuppress > PSL_DEBUG)
		return;
	va_list args;
	
	va_start(args, fmt);
	printf(
		"%s"
		"\e[90;2m"
		"[\e[0;90mdebg\e[90;2m]:\t"
		"\e[m",
		sPrintfPrefix
	);
	vprintf(
		fmt,
		args
	);
	printf("\n");
	va_end(args);
}

void printf_warning(const char* fmt, ...) {
	if (gPrintfSuppress >= PSL_NO_WARNING)
		return;
	va_list args;
	
	va_start(args, fmt);
	printf(
		"%s"
		"\e[90;2m"
		"[\e[0;93mwarn\e[90;2m]:\t"
		"\e[m",
		sPrintfPrefix
	);
	vprintf(
		fmt,
		args
	);
	printf("\n");
	va_end(args);
}

void printf_error(const char* fmt, ...) {
	if (gPrintfSuppress < PSL_NO_ERROR) {
		va_list args;
		
		va_start(args, fmt);
		printf(
			"%s"
			"\e[90;2m"
			"[\e[0;91merr!\e[90;2m]:\t"
			"\e[m",
			sPrintfPrefix
		);
		vprintf(
			fmt,
			args
		);
		printf("\n");
		va_end(args);
	}
	exit(EXIT_FAILURE);
}

void printf_info(const char* fmt, ...) {
	if (gPrintfSuppress >= PSL_NO_INFO)
		return;
	va_list args;
	
	va_start(args, fmt);
	printf(
		"%s"
		"\e[90;2m"
		"[\e[0;94minfo\e[90;2m]:\t"
		"\e[m",
		sPrintfPrefix
	);
	vprintf(
		fmt,
		args
	);
	printf("\n");
	va_end(args);
}

void printf_WinFix() {
	#ifdef _WIN32
	system("\0");
	#endif
}

/* 👺 LIB 👺 */
void* Lib_MemMem(const void* haystack, size_t haystackSize, const void* needle, size_t needleSize) {
	if (haystack == NULL || needle == NULL)
		return NULL;
	register char* cur, * last;
	const char* cl = (const char*)haystack;
	const char* cs = (const char*)needle;
	
	/* we need something to compare */
	if (haystackSize == 0 || needleSize == 0)
		return NULL;
	
	/* "s" must be smaller or equal to "l" */
	if (haystackSize < needleSize)
		return NULL;
	
	/* special case where s_len == 1 */
	if (needleSize == 1)
		return memchr(haystack, (int)*cs, haystackSize);
	
	/* the last position where its possible to find "s" in "l" */
	last = (char*)cl + haystackSize - needleSize;
	
	for (cur = (char*)cl; cur <= last; cur++) {
		if (*cur == *cs && memcmp(cur, cs, needleSize) == 0)
			return cur;
	}
	
	return NULL;
}

void Lib_ByteSwap(void* src, s32 size) {
	u32 buffer[16] = { 0 };
	u8* temp = (u8*)buffer;
	u8* srcp = src;
	
	for (s32 i = 0; i < size; i++) {
		temp[size - i - 1] = srcp[i];
	}
	
	for (s32 i = 0; i < size; i++) {
		srcp[i] = temp[i];
	}
}

void* Lib_Malloc(void* data, s32 size) {
	data = malloc(size);
	
	if (data == NULL) {
		printf_error("Could not allocate [0x%X] bytes.", size);
	}
	
	return data;
}

void* Lib_Realloc(void* data, s32 size) {
	data = realloc(data, size);
	
	if (data == NULL) {
		printf_error("Could not reallocate to [0x%X] bytes.", size);
	}
	
	return data;
}

s32 Lib_ParseArguments(char* argv[], char* arg, u32* parArg) {
	s32 i = 1;
	
	*parArg = 0;
	
	while (argv[i] != NULL) {
		if (Lib_MemMem(argv[i], strlen(arg), arg, strlen(arg))) {
			*parArg =  i + 1;
			
			return i + 1;
		}
		
		i++;
	}
	
	return 0;
}

/* 👺 FILE 👺 */
s32 File_Load(void** dst, char* filepath) {
	s32 size;
	
	FILE* file = fopen(filepath, "r");
	
	if (file == NULL) {
		printf_error("Could not open file [%s]", filepath);
	}
	
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	if (*dst == NULL) {
		*dst = Lib_Malloc(*dst, size);
		if (*dst == NULL) {
			printf_error("Failed to malloc [0x%X] bytes to store data from [%s].", size, filepath);
		}
	}
	rewind(file);
	fread(*dst, sizeof(char), size, file);
	fclose(file);
	
	return size;
}

void File_Save(char* filepath, void* src, s32 size) {
	FILE* file = fopen(filepath, "w");
	
	if (file == NULL) {
		printf_error("Failed to fopen file [%s].", filepath);
	}
	
	fwrite(src, sizeof(u8), size, file);
	fclose(file);
}

s32 File_Load_ReqExt(void** dst, char* filepath, const char* ext) {
	if (Lib_MemMem(filepath, strlen(filepath), ext, strlen(ext))) {
		return File_Load(dst, filepath);
	}
	printf_error("[%s] does not match extension [%s]", filepath, ext);
	
	return 0;
}

void File_Save_ReqExt(char* filepath, void* src, s32 size, const char* ext) {
	if (Lib_MemMem(filepath, strlen(filepath), ext, strlen(ext))) {
		File_Save(filepath, src, size);
		
		return;
	}
	
	printf_error("[%s] does not match extension [%s]", src, ext);
}

/* 👺 MEMFILE 👺 */
MemFile MemFile_Initialize() {
	return (MemFile) { 0 };
}

void MemFile_Malloc(MemFile* memFile, u32 size) {
	memset(memFile, 0, sizeof(MemFile));
	memFile->data = malloc(size);
	
	if (memFile->data == NULL) {
		printf_error("Failed to malloc [0x%X] bytes.", size);
	}
	
	memFile->memSize = size;
}

void MemFile_Realloc(MemFile* memFile, u32 size) {
	printf_debugExt("memSize:", memFile->memSize);
	printf_debug("reqSize", size);
	// Make sure to have enough space
	if (size < memFile->memSize + 0x10000) {
		size += 0x10000;
	}
	
	memFile->data = realloc(memFile->data, size);
	memFile->memSize = size;
	printf_debug("newSize", size);
}

void MemFile_Rewind(MemFile* memFile) {
	memFile->seekPoint = 0;
}

void MemFile_Write(MemFile* dest, void* src, u32 size) {
	if (dest->seekPoint + size > dest->memSize) {
		printf_error("DataSize exceeded MemSize while writing to MemFile.\n\tDataSize: [0x%X]\n\tMemSize: [0x%X]", dest->dataSize, dest->memSize);
	}
	if (dest->seekPoint + size > dest->dataSize) {
		dest->dataSize = dest->seekPoint + size;
	}
	memcpy(&dest->cast.u8[dest->seekPoint], src, size);
	dest->seekPoint += size;
}

void MemFile_LoadFile(MemFile* memFile, char* filepath) {
	u32 tempSize;
	FILE* file = fopen(filepath, "rb");
	
	if (file == NULL) {
		printf_error("Failed to fopen file [%s].", filepath);
	}
	
	fseek(file, 0, SEEK_END);
	tempSize = ftell(file);
	
	if (memFile->data == NULL) {
		MemFile_Malloc(memFile, tempSize);
		memFile->memSize = memFile->dataSize = tempSize;
		if (memFile->data == NULL) {
			printf_error("Failed to malloc MemFile.\n\tAttempted size is [0x%X] bytes to store data from [%s].", tempSize, filepath);
		}
	} else if (memFile->memSize < tempSize) {
		MemFile_Realloc(memFile, tempSize);
	}
	memFile->dataSize = tempSize;
	
	rewind(file);
	fread(memFile->data, 1, memFile->dataSize, file);
	fclose(file);
	
	printf_debugExt("File: [%s]", filepath);
	printf_debug("Ptr: %08X", memFile->data);
	printf_debug("Size: %08X", memFile->dataSize);
}

void MemFile_SaveFile(MemFile* memFile, char* filepath) {
	FILE* file = fopen(filepath, "w");
	
	if (file == NULL) {
		printf_error("Failed to fopen file [%s] for writing.", filepath);
	}
	
	fwrite(memFile, sizeof(u8), memFile->dataSize, file);
	fclose(file);
}

void MemFile_LoadFile_ReqExt(MemFile* memFile, char* filepath, const char* ext) {
	if (Lib_MemMem(filepath, strlen(filepath), ext, strlen(ext))) {
		MemFile_LoadFile(memFile, filepath);
		
		return;
	}
	printf_error("[%s] does not match extension [%s]", filepath, ext);
}

void MemFile_SaveFile_ReqExt(MemFile* memFile, char* filepath, s32 size, const char* ext) {
	if (Lib_MemMem(filepath, strlen(filepath), ext, strlen(ext))) {
		MemFile_SaveFile(memFile, filepath);
		
		return;
	}
	
	printf_error("[%s] does not match extension [%s]", filepath, ext);
}

void MemFile_Free(MemFile* memFile) {
	if (memFile->data) {
		free(memFile->data);
		
		memset(memFile, 0, sizeof(MemFile));
	}
}

/* 👺 STRING 👺 */
u32 String_HexStrToInt(char* string) {
	return strtol(string, NULL, 16);
}

u32 String_NumStrToInt(char* string) {
	return strtol(string, NULL, 10);
}

f64 String_NumStrToF64(char* string) {
	return strtod(string, NULL);
}

s32 String_GetLineCount(char* str) {
	s32 line = 1;
	s32 i = 0;
	
	while (str[i++] != '\0') {
		if (str[i] == '\n' && str[i + 1] != '\0')
			line++;
	}
	
	return line;
}

char* String_GetLine(char* str, s32 line) {
	static char buffer[1024] = { 0 };
	s32 iLine = 0;
	s32 i = 0;
	s32 j = 0;
	
	memset(buffer, 0, 1024);
	
	while (str[i] != '\0') {
		j = 0;
		if (str[i] != '\n') {
			while (str[i + j] != '\n' && str[i + j] != '\0') {
				j++;
			}
			
			iLine++;
			
			if (iLine == line) {
				break;
			}
			
			i += j;
		} else {
			i++;
		}
	}
	
	memcpy(buffer, &str[i], j);
	
	return buffer;
}

char* String_GetWord(char* str, s32 word) {
	static char buffer[1024] = { 0 };
	s32 iWord = 0;
	s32 i = 0;
	s32 j = 0;
	
	memset(buffer, 0, 1024);
	
	while (str[i] != '\0') {
		j = 0;
		if (str[i] != ' ' && str[i] != '\t') {
			while (str[i + j] != ' ' && str[i + j] != '\t' && str[i + j] != '\0') {
				j++;
			}
			
			iWord++;
			
			if (iWord == word) {
				break;
			}
			
			i += j;
		} else {
			i++;
		}
	}
	
	memcpy(buffer, &str[i], j);
	
	return buffer;
}

void String_CaseToLow(char* s, s32 i) {
	for (s32 k = 0; k < i; k++) {
		if (s[k] >= 'A' && s[k] <= 'Z') {
			s[k] = s[k] + 32;
		}
	}
}

void String_CaseToUp(char* s, s32 i) {
	for (s32 k = 0; k < i; k++) {
		if (s[k] >= 'a' && s[k] <= 'z') {
			s[k] = s[k] - 32;
		}
	}
}

void String_GetSlashAndPoint(char* src, s32* slash, s32* point) {
	s32 strSize = strlen(src);
	
	for (s32 i = strSize; i > 0; i--) {
		if (*point == 0 && src[i] == '.') {
			*point = i;
		}
		if (src[i] == '/' || src[i] == '\\') {
			*slash = i;
			break;
		}
	}
}

void String_GetPath(char* dst, char* src) {
	s32 point = 0;
	s32 slash = 0;
	
	String_GetSlashAndPoint(src, &slash, &point);
	
	if (slash == 0)
		slash = -1;
	
	memset(dst, 0, slash + 2);
	memcpy(dst, src, slash + 1);
}

void String_GetBasename(char* dst, char* src) {
	s32 point = 0;
	s32 slash = 0;
	
	String_GetSlashAndPoint(src, &slash, &point);
	
	if (slash == 0)
		slash = -1;
	
	memset(dst, 0, point - slash);
	memcpy(dst, &src[slash + 1], point - slash - 1);
}

void String_GetFilename(char* dst, char* src) {
	s32 point = 0;
	s32 slash = 0;
	
	String_GetSlashAndPoint(src, &slash, &point);
	
	if (slash == 0)
		slash = -1;
	
	memset(dst, 0, strlen(src) - slash);
	memcpy(dst, &src[slash + 1], strlen(src) - slash - 1);
}