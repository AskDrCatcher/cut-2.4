/*
 * CUT 2.4
 * Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006 Samuel A. Falvo II, et. al.
 * See LICENSE for details.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define DO_NOT_PROCESS        "..."

#define SEARCH_TOKEN_TEST     "__CUT__"
#define SEARCH_TOKEN_BRINGUP  "__CUT_BRINGUP__"
#define SEARCH_TOKEN_TAKEDOWN "__CUT_TAKEDOWN__"

#define MAX_SYMBOL_LENGTH 256  /* arbitrary */
#define MAX_LINE_LENGTH   1024 /* arbitrary */

#define SEARCH_TOKEN_TEST_LENGTH       sizeof( SEARCH_TOKEN_TEST )-1
#define SEARCH_TOKEN_BRINGUP_LENGTH    sizeof( SEARCH_TOKEN_BRINGUP )-1
#define SEARCH_TOKEN_TAKEDOWN_LENGTH   sizeof( SEARCH_TOKEN_TAKEDOWN )-1

typedef enum TestType {
   TYPE_TEST = 0,
   TYPE_BRINGUP = 1,
   TYPE_TAKEDOWN = 2
} TestType;

typedef struct TestItem {
   char name[MAX_SYMBOL_LENGTH];
   enum TestType type;
   struct TestItem *next;
} TestItem;

/* globals */

TestItem *testList = 0;
FILE *outfile;
extern char *libCUT[]; /* defined in cuttemplate.c file */
static char *headerPath = NULL;

static int g_count, g_ready, g_index;  /* Used by filename globbing support for windows */
static char **g_wildcards, g_fileName[MAX_LINE_LENGTH];

TestItem *FindFirstMatch( TestItem *current, char *basis_name, int basis_type )
{
   while ( current )
   {
      if ( !strcmp(current->name,basis_name) && current->type == basis_type )
         return current;
      current = current->next;
   }
   return 0;
}

int NameAndTypeInTestList( char *name, TestType type )
{
   return 0 != FindFirstMatch(testList,name,type);
}

void AppendToTestList( char *name, TestType type )
{
   struct TestItem *current = testList;
   if ( !current )
      current = testList = malloc( sizeof( *testList) );
   else
   {
      while ( current->next ) current = current->next;
      current->next = malloc(sizeof( *testList));
      current = current->next;
   }
   
   current->next = 0;
   strcpy(current->name, name);
   current->type = type;
}

void InsertNameAndTypeIntoTestList( char *name, TestType type )
{
    if ( !NameAndTypeInTestList( name, type ) )
        AppendToTestList( name, type );
}

int CharacterIsDigit(char ch)
{
    return ( ( ch >= '0') && ( ch <= '9' ) );
}

int CharacterIsUppercase(char ch)
{
    return ( ( ch >= 'A' ) && ( ch <= 'Z' ) );
}

int CharacterIsLowercase(char ch)
{
    return ( ( ch >= 'a' ) && ( ch <= 'z' ) );
}

int CharacterIsAlphabetic(char ch)
{
    return CharacterIsUppercase(ch) || CharacterIsLowercase(ch) || ( ch == '_' );
}

int CharacterIsAlphanumeric( char ch )
{
    return CharacterIsDigit(ch) || CharacterIsAlphabetic(ch);
}

void ProcessGenericFunction( char *line, int position, 
                            TestType type, int tokenDisplacement )
{
    char name[MAX_SYMBOL_LENGTH] = "";
    int maxLength = strlen( line ) - 1, offset=0;
    position = position + tokenDisplacement;
    
    while ( CharacterIsAlphanumeric(line[position]) 
       && (position<maxLength) && (offset<MAX_SYMBOL_LENGTH) )
    {
        name[offset++] = line[position++];
        name[offset] = 0;
    }

    InsertNameAndTypeIntoTestList( name, type );
}

void ProcessBringupFunction( char *line, int position )
{
    ProcessGenericFunction( line, position, TYPE_BRINGUP, SEARCH_TOKEN_BRINGUP_LENGTH );
}

void ProcessTestFunction( char *line, int position )
{
    ProcessGenericFunction( line, position, TYPE_TEST, SEARCH_TOKEN_TEST_LENGTH );
}

void ProcessTakedownFunction( char *line, int position )
{
    ProcessGenericFunction( line, position, TYPE_TAKEDOWN, SEARCH_TOKEN_TAKEDOWN_LENGTH );
}


int OffsetOfSubstring( char *line, char *token )
{
    char *inset = strstr(line,token);
    
    if ( !inset ) return -1;
    else return inset - line;
}

void CallIfSubstringFound( char *line, char *token, void (*function)(char*,int) )
{
    int index = OffsetOfSubstring( line, token );
    if ( index != -1 )
        function( line, index );
}

void ProcessSourceFile( char *filename )
{
   FILE *source;
   char line[MAX_LINE_LENGTH];
  
   if( strcmp( filename, DO_NOT_PROCESS ) != 0 )
   {

      source = fopen(filename,"r");
      
      while ( fgets(line,MAX_LINE_LENGTH,source) )
      {
         CallIfSubstringFound( line, SEARCH_TOKEN_BRINGUP, ProcessBringupFunction );
         CallIfSubstringFound( line, SEARCH_TOKEN_TEST, ProcessTestFunction );
         CallIfSubstringFound( line, SEARCH_TOKEN_TAKEDOWN, ProcessTakedownFunction );
      }
      
      fclose(source);
   }
}

void EmitExternDeclarationFor( char *name, char *prefix )
{
    fprintf( outfile, "extern void %s%s( void );\n", prefix, name );
}

void Emit(char *text)
{
   fprintf(outfile, "%s\n", text);
}

void EmitInclude( char *headerFile )
{
   fprintf( outfile, "#include <%s>\n", headerFile );
}

void BlankLine()
{
    Emit( "" );
}

void ListExternalFunctions()
{
   TestItem *current = testList;
   while ( current )
   {
      if (current->type == TYPE_TEST)
         EmitExternDeclarationFor( current->name, SEARCH_TOKEN_TEST );
      else if (current->type == TYPE_BRINGUP)
         EmitExternDeclarationFor( current->name, SEARCH_TOKEN_BRINGUP );
      else if (current->type == TYPE_TAKEDOWN)
         EmitExternDeclarationFor( current->name, SEARCH_TOKEN_TAKEDOWN );
      current = current->next;
   }
   
   BlankLine();
}

void EmitLibrary(void)
{
   int i=0;
   while ( libCUT[i] )
   {
      if( i == 10 )   /* #include <cut-2/cut.h> */
      {
         if( headerPath )
            EmitInclude( headerPath );
         else
            EmitInclude( "cut-2/cut.h" );
      }
      else
         Emit(libCUT[i]);

      i++;
   }
}

void ListHeaderFiles(void)
{
    EmitLibrary();
    BlankLine();
    BlankLine();
}

void EmitIndented(int indent,char *format, ...)
{
   va_list v;
   /* Print two spaces per level of indentation. */
   fprintf( outfile, "%*s", indent*2, "" );
   
   va_start(v,format);
   vfprintf( outfile, format, v );
   va_end(v);
   
   fprintf( outfile, "\n" );
}

void EmitBringup(int indent,char *name)
{
    BlankLine();
    EmitIndented(indent, "cut_start( \"group-%s\", __CUT_TAKEDOWN__%s );", 
           name, name );
    EmitIndented(indent, "__CUT_BRINGUP__%s();", name );
    EmitIndented(indent, "cut_check_errors();");
}

void EmitTest(int indent,char *name)
{
    EmitIndented(indent, "cut_start( \"%s\", 0 );", name );
    EmitIndented(indent, "__CUT__%s();", name );
    EmitIndented(indent, "cut_end( \"%s\" );", name );
}

void EmitTakedown(int indent,char *name)
{
    EmitIndented(indent, "cut_end( \"group-%s\" );", name );
    EmitIndented(indent, "__CUT_TAKEDOWN__%s();", name );
    BlankLine();
}

void EmitUnitTesterBody()
{
    int indent=0;
    TestItem *test;
    Emit( "int main( int argc, char *argv[] )\n{" );
    Emit( "  if ( argc == 1 )" );
    Emit( "    cut_init( -1 );" );
    Emit( "  else cut_init( atoi( argv[1] ) );" );
    BlankLine();
    
    indent = 1;
    test = testList;
    while ( test )
    {
      if (test->type == TYPE_BRINGUP)
      {
         EmitBringup(indent,test->name);
         indent ++;
      }

      if (test->type == TYPE_TEST)
         EmitTest(indent,test->name);

      if (test->type == TYPE_TAKEDOWN)
      {
         indent --;
         EmitTakedown(indent,test->name);
      }
      test = test->next;
    }
    
    BlankLine();
    Emit( "  cut_break_formatting();" );
    Emit( "  printf(\"Done.\");" );
    Emit( "  return 0;\n}\n" );
}

void EmitCutCheck()
{
    Emit( "/* Automatically generated: DO NOT MODIFY. */" );
    ListHeaderFiles();
    BlankLine();
    ListExternalFunctions();
    BlankLine();
    EmitUnitTesterBody();
}

#if defined(_MSC_VER) // version tested for MS Visual C++ 6.0
#include <io.h>

void FileName(char *base)
{
   char *pos = strrchr(g_wildcards[g_index],'/');
   char *pos2 = strrchr(g_wildcards[g_index],'\\');
   if ( !pos || pos2 > pos )
      pos = pos2;
   
   if ( pos )
   {
      size_t length = 1 + pos - g_wildcards[g_index];
      strncpy(g_fileName,g_wildcards[g_index],length);
      g_fileName[length] = 0;
   }
   else g_fileName[0] = 0;
   strcat(g_fileName,base);
}

int LoadArgument(void)
{
   static struct _finddata_t fd;
   static long handle = -1;
   
   if ( g_index>=g_count )
      return 0;
   
   if ( -1 == handle )
   {
      handle = _findfirst(g_wildcards[g_index], &fd);
      if ( -1 == handle ) // there MUST be at least a first match for each wildcard.
         return 0;
      FileName(fd.name);
      return 1;
   }
   else if ( 0 == _findnext(handle,&fd) )
   { // there's a 'next' filename
      FileName(fd.name);
      return 1;
   }
   else
   { // this wasn't the first filename, and there isn't a next.
      _findclose(handle);
      handle = -1;
      g_index++;
      return LoadArgument();
   }
}

#endif
#if defined(__POSIX__)

void FileName( char *base )
{
    strncpy( g_fileName, base, MAX_LINE_LENGTH );
    g_fileName[ MAX_LINE_LENGTH - 1 ] = 0;
}

int LoadArgument( void )
{
   if ( g_index >= g_count )
      return 0;

   FileName( g_wildcards[g_index] );
   g_index++;  /* MUST come after FileName() call; bad code smell */
   return 1;
}

#endif

void StartArguments( int starting, int count, char *array[] )
{
   g_index = starting;
   g_count = count;
   g_wildcards = array;

   g_ready = LoadArgument();
}

int NextArgument( void )
{
   if( g_ready )
      g_ready = LoadArgument();

   return g_ready;
}

char *GetArgument( void )
{
   if( g_ready )
      return g_fileName;

   return NULL;
}

void DetermineCutHLocation( int argc, char *argv[] )
{
   int i;

   i = 0;
   while( i < argc )
   {
      if( ( argv[i+1] != NULL ) && ( strcmp( argv[i], "-i" ) == 0 ) )
      {
         headerPath = (char *)( malloc( strlen( argv[i+1] + 2 ) ) );
         strcpy( headerPath, argv[i+1] );

         argv[i] = argv[i+1] = DO_NOT_PROCESS;
         return;
      }

      i++;
   }

   headerPath = NULL;
}

void EstablishOutputFile( int argc, char *argv[] )
{
   int i;

   i = 0;
   while( i < argc )
   {
      if( ( argv[i+1] != NULL ) && ( strcmp( argv[i], "-o" ) == 0 ) )
      {
         outfile = fopen( argv[i+1], "wb+" );
         if( outfile == NULL )
            fprintf( stderr, "ERROR: Can't open %s for writing.\n", argv[i+1] );
         
         argv[i] = argv[i+1] = DO_NOT_PROCESS;

         return;
      }

      i++;
   }

   outfile = stdout;
}

int main( int argc,char *argv[] )
{
   char *filename;

   if ( argc < 2 )
   {
      fprintf(
              stderr,
              "USAGE:\n"
              "   %s [options] <input file> [<input file> [...]]\n"
              "\n"
              "OPTIONS:\n"
              "   -o filename   Specifies output file.\n"
              "\n"
              "NOTES:\n"
              "   If -o is left unspecified, output defaults to stdout.\n",
              argv[0]
             );
      return 3;
   }

   EstablishOutputFile( argc, argv );
   DetermineCutHLocation( argc, argv );

   /* Skip the executable's name and the output filename. */
   StartArguments(0,argc,argv);

   /* Consume the rest of the arguments, one at a time. */
   while ( NextArgument() )
   {
      filename = GetArgument();

      if( strcmp( filename, DO_NOT_PROCESS ) )
      {
         fprintf( stderr, "  - parsing '%s'... ", filename);
         ProcessSourceFile( filename );
         fprintf( stderr, "done.\n");
      }
   }
   
   EmitCutCheck();
   fflush(outfile);
   fclose(outfile);
   return 0;
}

