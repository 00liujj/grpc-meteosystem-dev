// -*- coding: UTF-8 -*-
//
// Модуль разбора аргументов командной строки.
//
// ВНИМАНИЕ! Этот модуль создан генератором кода ODF.
// Настоятельно не рекомендуется вносить в него какие-либо изменения
// вручную!
//

/** @page page_cli Параметры командной строки

@section sec_17057ae621848dd72e784cc6d5a12000 Управление режимом запуска службы

	Программа реализована как служба операционной системы. Это означает, что
	она запускается автоматически при старте ОС и функционирует в фоновом
	режиме до тех пор, пока система не будет остановлена или перезагружена.

	Однако для отладочных и диагностических целей может потребоваться запуск
	программы в виде приложения, с выводом сообщений на консоль. Это поведение может
	быть обеспечено нижеперечисленными параметрами командной строки.

	Предусмотрены также параметры, позволяющие программе осуществлять
	саморегистрацию службы операционной системы в обход стандартного механизма
	установки.
	
@subsection ss_opt_app --app 

	Запуск программы в режиме приложения.
	Программа запускается с правами текущего пользователя и с использованием
	его окружения. На консоль выводятся различные диагностические сообщения
	(см. @ref options_group_logging "параметры управления журналированием".).
	
@subsection ss_opt_no_console_title --no-console-title 

	Не устанавливать заголовок консольного окна в режиме приложения.

	По умолчанию при запуске программы в режиме приложения заголовок
	его консольного окна устанавливается в отображаемое наименование службы.
	Данный параметр позволяет отключить эту возможность.
	
@subsection ss_opt_install --install 

	Установить программу в качестве службы операционной системы.

	Пользователь, запускающий программу, должен обладать соответствующими правами.
	
@subsection ss_opt_uninstall --uninstall 

	Удалить службу операционной системы, соответствующую программе.

	Пользователь, запускающий программу, должен обладать соответствующими правами.
	
@section sec_18f9e99a81137d7647057689d809c6ec Параметры установки службы (только совместно с --install)

	Эта группа параметров обеспечивает дополнительное управление
	установкой службы и позволяет изменить, например, имя устанавливаемой
	службы или командную строку её запуска.

	Параметры этой группы могут быть использованы только совместно
	с параметром @ref ss_opt_install.
	
@subsection ss_opt_servicename --servicename строка
Системное имя службы

@a Значение @a по @a умолчанию: не задано 
@subsection ss_opt_displayname --displayname строка
Отображаемое имя службы

@a Значение @a по @a умолчанию: не задано 
@subsection ss_opt_args --args строка

	Используется для установки системной службы с определённой командной
	строкой запуска.

	Если аргументы командной строки необходимо отделять кавычками друг от
	друга, в качестве разделителей нужно использовать одинарные кавычки,
	которые потом программно будут заменены на двойные.

	Пример: --args=" --path='/opt/fort/src/common/' --log-no-bin"
	

@a Значение @a по @a умолчанию: не задано 
@section sec_f568849c35de99189a3fffd02fdf5454 Управление журналированием

  @anchor options_group_logging

  Эта группа параметров обеспечивает управление журналированием, которое
  программа осуществляет через
  @ref liblog_mainpage "библиотеку ведения журналов".

  Параметры этой группы предназначены преимущественно для технологических
  целей.
  
@subsection ss_opt_log_no_con --log-no-con 

    Параметр позволяет отключить журналирование на консоль при работе
    программы в режиме приложения (см. @ref ss_opt_app).
    
@subsection ss_opt_log_no_txt --log-no-txt 

    Параметр позволяет отключить журналирование в текстовые файлы
    в директории /opt/fort/var/log.
    
@subsection ss_opt_log_no_bin --log-no-bin 

    Параметр позволяет отключить журналирование в двоичные файлы.
    @warning Так как в автоматической пересылке протокольных данных участвуют
    только двоичные файлы, использование этого параметра повлечёт за собой
    невозможность передачи данных в централизованное хранилище.
    
@subsection ss_opt_log_bin_exclusive --log-bin-exclusive 

    Параметр позволяет осуществлять журналирование не в общий каталог
    двоичных файлов _common, а в собственный, формируемый на основании
    имени процесса или службы.

    Если задан параметр @ref ss_opt_log_no_bin, параметр игнорируется.
    
@subsection ss_opt_log_maxlevel -L строка,  --log-maxlevel строка

    Параметр позволяет изменить максимальный уровень важности журналируемых
    сообщений. Например, можно подавить вывод всех сообщений, кроме сообщений
    об ошибках, или дополнить вывод отладочными сообщениями.

    Возможные значения: @a debug (все сообщения),
    @a info (информационные сообщения и более важные),
    @a warn (предупреждения и сообщения об ошибках),
    @a error (только сообщения об ошибках).
    

@a Значение @a по @a умолчанию: info 
@subsection ss_opt_log_no_color --log-no-color 

    Параметр позволяет отключить выделение цветом уровней важности сообщений
    при журналировании на консоль.

    Параметр имеет смысл только при работе в режиме приложения
    (см. @ref ss_opt_app).

    Если задан параметр @ref ss_opt_log_no_con, параметр игнорируется.
    
@subsection ss_opt_log_con_encoding --log-con-encoding строка

    Параметр позволяет указать кодировку, используемую при журналировании на
    консоль.

    По умолчанию используется кодировка, принятая по умолчанию для консоли
    операционной системы (для МСВС 3.0 - koi8-r, для MS Windows - CP866).

    Параметр имеет смысл только при работе в режиме приложения
    (см. @ref ss_opt_app).

    Если задан параметр @ref ss_opt_log_no_con, параметр игнорируется.
    

@a Значение @a по @a умолчанию: не задано 
@section sec_0373d10187167d1adb17c4cc1916e954 Разное

@subsection ss_opt_help -h ,  --help 

	При запуске с этим параметром программа выводит на консоль краткую справку
	по допустимым аргументам командной строки и завершает свою работу, не
	выполняя более никаких действий.
	
@subsection ss_opt_version -v ,  --version 

	При запуске с этим параметром программа выводит на консоль информацию
	о версии и завершает свою работу, не выполняя более никаких действий.
	

@a Значение @a по @a умолчанию: не задано 

 */

#if (defined(WIN32) || defined(_WIN32)) && defined(ODF_GUI)
# include <windows.h>
# include "common/include/snprintf.h"
#endif
#include <stdlib.h>
#include <stdio.h>

#include "common/include/spmportable.h"

/*
#include "odf_meteo-point-tester.h"
*/
#define ODF_BOOL    int

#define chSTR2(x)      #x
#define chSTR(x)   chSTR2(x)




// Запуск в режиме приложения.
ODF_BOOL opt_app = 0;
// Не устанавливать заголовок консольного окна в режиме приложения.
ODF_BOOL opt_no_console_title = 0;
// Установить программу в качестве службы ОС.
ODF_BOOL opt_install = 0;
// Удалить службу ОС, соответствующую программе.
ODF_BOOL opt_uninstall = 0;
// Системное имя службы.
char * opt_servicename = "";
// Отображаемое имя службы.
char * opt_displayname = "";
// Аргументы командной строки исполняемого файла.
char * opt_args = "";
// Отключить журналирование на консоль.
ODF_BOOL opt_log_no_con = 0;
// Отключить журналирование в текстовые файлы.
ODF_BOOL opt_log_no_txt = 0;
// Отключить журналирование в двоичные файлы.
ODF_BOOL opt_log_no_bin = 0;
// Осуществлять журналирование в собственный каталог двоичных файлов.
ODF_BOOL opt_log_bin_exclusive = 0;
// Установить максимальный уровень важности журналируемых сообщений (debug, info, notice, warn, error).
char * opt_log_maxlevel = "info";
// Отключить раскраску консольного вывода.
ODF_BOOL opt_log_no_color = 0;
// Установить кодировку консоли.
char * opt_log_con_encoding = "";
// Вывести справку по параметрам запуска и завершить работу.
ODF_BOOL opt_help = 0;
// Вывести информацию о версии программы и завершить работу.
int opt_version = 0;


#ifdef FORT_VERSION_INFO
#include "./meteo-point-tester.version_i.c"
static
void print_version_and_exit(const char *optarg) {
  if (optarg == NULL) {
#ifdef ODF_GUI
    char buf[1024] = "";
    snprintf(buf, sizeof(buf),
#else
    printf(
#endif
      "%s%s%s\n%s %d.%d.%d.%d %s\n(c) %s %s\n",
      executable_version_product? executable_version_product : "",
      executable_version_product? "\n" : "",
      executable_version_description, executable_version_literal_version,
      executable_version_major, executable_version_minor,
      executable_version_patch, executable_version_revision,
      executable_version_extra,
      executable_version_copyright, executable_version_copyright_years);
#ifdef ODF_GUI
    OemToCharBuffA(buf, buf, sizeof(buf));
    MessageBoxA(NULL, buf, "\xc8\xed\xf4\xee\xf0\xec\xe0\xf6\xe8\xff\x20\xee\x20\xe2\xe5\xf0\xf1\xe8\xe8", MB_OK);
#endif
  } else {
    printf("%d.%d.%d.%d\n",
      executable_version_major, executable_version_minor,
      executable_version_patch, executable_version_revision);
  }
  exit(0);
}
#else
static
void print_version_and_exit(const char *optarg) {
  SPM_UNUSED(optarg)
}
#endif  // FORT_VERSION_INFO


int check_options(int argc, char* argv[]) {
  int res;

  char optstring[] = "L:hv::";
  struct option long_options[] = {

    { "app", 0, NULL, 127 },
    { "no-console-title", 0, NULL, 128 },
    { "install", 0, NULL, 129 },
    { "uninstall", 0, NULL, 130 },
    { "servicename", 1, NULL, 131 },
    { "displayname", 1, NULL, 132 },
    { "args", 1, NULL, 133 },
    { "log-no-con", 0, NULL, 134 },
    { "log-no-txt", 0, NULL, 135 },
    { "log-no-bin", 0, NULL, 136 },
    { "log-bin-exclusive", 0, NULL, 137 },
    { "log-maxlevel", 1, NULL, 'L' },
    { "log-no-color", 0, NULL, 139 },
    { "log-con-encoding", 1, NULL, 140 },
    { "help", 0, NULL, 'h' },
    { "version", 2, NULL, 'v' },

    { NULL, 0, NULL, 0 }
  };

  // Подавляем вывод англоязычных сообщений об ошибках
  set_opterr(0);

  // Начинаем анализ...
  while((res=getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
    switch (res) {

      case 127: opt_app = 1;  break;
      case 128: opt_no_console_title = 1;  break;
      case 129: opt_install = 1;  break;
      case 130: opt_uninstall = 1;  break;
      case 131: if(get_optarg() == NULL) goto invalid_op; else { opt_servicename = get_optarg(); } break;
      case 132: if(get_optarg() == NULL) goto invalid_op; else { opt_displayname = get_optarg(); } break;
      case 133: if(get_optarg() == NULL) goto invalid_op; else { opt_args = get_optarg(); } break;
      case 134: opt_log_no_con = 1;  break;
      case 135: opt_log_no_txt = 1;  break;
      case 136: opt_log_no_bin = 1;  break;
      case 137: opt_log_bin_exclusive = 1;  break;
      case 'L': if(get_optarg() == NULL) goto invalid_op; else { opt_log_maxlevel = get_optarg(); } break;
      case 139: opt_log_no_color = 1;  break;
      case 140: if(get_optarg() == NULL) goto invalid_op; else { opt_log_con_encoding = get_optarg(); } break;
      case 'h': opt_help = 1;  break;
      case 'v': print_version_and_exit(get_optarg()); break;

      case '\?': goto invalid_op;
    }  // switch
  }  // while

  return 0;

invalid_op:
  return get_optind() - 1;
}


int check_options_ex(int * pArgc, char ** pArgv[]) {
  int res;

  char optstring[] = "L:hv::";
  struct option long_options[] = {

    { "app", 0, NULL, 127 },
    { "no-console-title", 0, NULL, 128 },
    { "install", 0, NULL, 129 },
    { "uninstall", 0, NULL, 130 },
    { "servicename", 1, NULL, 131 },
    { "displayname", 1, NULL, 132 },
    { "args", 1, NULL, 133 },
    { "log-no-con", 0, NULL, 134 },
    { "log-no-txt", 0, NULL, 135 },
    { "log-no-bin", 0, NULL, 136 },
    { "log-bin-exclusive", 0, NULL, 137 },
    { "log-maxlevel", 1, NULL, 'L' },
    { "log-no-color", 0, NULL, 139 },
    { "log-con-encoding", 1, NULL, 140 },
    { "help", 0, NULL, 'h' },
    { "version", 2, NULL, 'v' },

    { NULL, 0, NULL, 0 }
  };

  // Подавляем вывод англоязычных сообщений об ошибках
  set_opterr(0);

  // Начинаем анализ...
  while((res=getopt_long(*pArgc,*pArgv,optstring,long_options,NULL)) != -1) {
    switch (res) {

      case 127: opt_app = 1;  break;
      case 128: opt_no_console_title = 1;  break;
      case 129: opt_install = 1;  break;
      case 130: opt_uninstall = 1;  break;
      case 131: if(get_optarg() == NULL) goto invalid_op; else { opt_servicename = get_optarg(); } break;
      case 132: if(get_optarg() == NULL) goto invalid_op; else { opt_displayname = get_optarg(); } break;
      case 133: if(get_optarg() == NULL) goto invalid_op; else { opt_args = get_optarg(); } break;
      case 134: opt_log_no_con = 1;  break;
      case 135: opt_log_no_txt = 1;  break;
      case 136: opt_log_no_bin = 1;  break;
      case 137: opt_log_bin_exclusive = 1;  break;
      case 'L': if(get_optarg() == NULL) goto invalid_op; else { opt_log_maxlevel = get_optarg(); } break;
      case 139: opt_log_no_color = 1;  break;
      case 140: if(get_optarg() == NULL) goto invalid_op; else { opt_log_con_encoding = get_optarg(); } break;
      case 'h': opt_help = 1;  break;
      case 'v': print_version_and_exit(get_optarg()); break;

      case '\?': goto invalid_op;
    }  // switch
  }  // while

  *pArgv += get_optind();
  *pArgc -= get_optind();

  return 0;

invalid_op:
  return get_optind() - 1;
}


char* help[] = {
"",
"Управление режимом запуска службы",
"---------------------------------",
"",
"--app                     Запуск в режиме приложения",
"",
"--no-console-title        Не устанавливать заголовок консольного окна в режиме",
"                          приложения",
"",
"--install                 Установить программу в качестве службы ОС",
"",
"--uninstall               Удалить службу ОС, соответствующую программе",
"",
"Параметры установки службы (только совместно с --install)",
"---------------------------------------------------------",
"",
"--servicename=строка      Системное имя службы",
"",
"--displayname=строка      Отображаемое имя службы",
"",
"--args=строка             Аргументы командной строки исполняемого файла",
"",
"Управление журналированием",
"--------------------------",
"",
"--log-no-con              Отключить журналирование на консоль",
"",
"--log-no-txt              Отключить журналирование в текстовые файлы",
"",
"--log-no-bin              Отключить журналирование в двоичные файлы",
"",
"--log-bin-exclusive       Осуществлять журналирование в собственный каталог",
"                          двоичных файлов",
"",
"--log-maxlevel=строка     Установить максимальный уровень важности",
"                          журналируемых сообщений (debug, info, notice, warn,",
"                          error) (по умолчанию \"info\")",
"",
"-L строка                 То же, что --log-maxlevel",
"",
"--log-no-color            Отключить раскраску консольного вывода",
"",
"--log-con-encoding=строка Установить кодировку консоли",
"",
"Разное",
"------",
"",
"--help                    Вывести справку по параметрам запуска и завершить",
"                          работу",
"",
"-h                        То же, что --help",
"",
"--version                 Вывести информацию о версии программы и завершить",
"                          работу",
"",
"-v                        То же, что --version",
NULL, NULL
};

