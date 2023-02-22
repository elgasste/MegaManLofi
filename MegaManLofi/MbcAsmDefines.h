#pragma once

// setting this to 1 will create ASM and MBC log files, which are super useful for debugging
#define MBCCOMP_ENABLELOGGER              0

#define MBCCOMP_MAINFUNC                  "MAIN"

#define MBCCOMP_REGF                      "REGF"
#define MBCCOMP_REGI                      "REGI"
#define MBCCOMP_CLEARFREG                 "CLEARFREG"
#define MBCCOMP_CLEARIREG                 "CLEARIREG"
#define MBCCOMP_ADDF                      "ADDF"
#define MBCCOMP_SUBF                      "SUBF"
#define MBCCOMP_MULF                      "MULF"
#define MBCCOMP_DIVF                      "DIVF"
#define MBCCOMP_ABSF                      "ABSF"
#define MBCCOMP_ADDI                      "ADDI"
#define MBCCOMP_SUBI                      "SUBI"
#define MBCCOMP_MULI                      "MULI"
#define MBCCOMP_DIVI                      "DIVI"
#define MBCCOMP_ABSI                      "ABSI"

#define MBCCOMP_FUNC                      "FUNC"
#define MBCCOMP_CALL                      "CALL"
#define MBCCOMP_RET                       "RET"
#define MBCCOMP_STOP                      "STOP"
#define MBCCOMP_GOTO                      "GOTO"
#define MBCCOMP_NOOP                      "NOOP"

#define MBCCOMP_IF_EQF                    "IF_EQF"
#define MBCCOMP_IF_LTF                    "IF_LTF"
#define MBCCOMP_IF_GTF                    "IF_GTF"
#define MBCCOMP_IF_LTEF                   "IF_LTEF"
#define MBCCOMP_IF_GTEF                   "IF_GTEF"
#define MBCCOMP_IF_EQI                    "IF_EQI"
#define MBCCOMP_IF_LTI                    "IF_LTI"
#define MBCCOMP_IF_GTI                    "IF_GTI"
#define MBCCOMP_IF_LTEI                   "IF_LTEI"
#define MBCCOMP_IF_GTEI                   "IF_GTEI"
#define MBCCOMP_ELSE                      "ELSE"
#define MBCCOMP_ENDIF                     "ENDIF"

#define MBCCOMP_WHILE_EQF                 "WHILE_EQF"
#define MBCCOMP_WHILE_LTF                 "WHILE_LTF"
#define MBCCOMP_WHILE_GTF                 "WHILE_GTF"
#define MBCCOMP_WHILE_LTEF                "WHILE_LTEF"
#define MBCCOMP_WHILE_GTEF                "WHILE_GTEF"
#define MBCCOMP_WHILE_EQI                 "WHILE_EQI"
#define MBCCOMP_WHILE_LTI                 "WHILE_LTI"
#define MBCCOMP_WHILE_GTI                 "WHILE_GTI"
#define MBCCOMP_WHILE_LTEI                "WHILE_LTEI"
#define MBCCOMP_WHILE_GTEI                "WHILE_GTEI"
#define MBCCOMP_BREAK                     "BREAK"
#define MBCCOMP_ENDWHILE                  "ENDWHILE"

#define MBCCOMP_GET_FRAMESECONDS                   "GET_FRAMESECONDS"
#define MBCCOMP_GET_PLAYERPOSITIONLEFT             "GET_PLAYERPOSITIONLEFT"
#define MBCCOMP_GET_PLAYERPOSITIONTOP              "GET_PLAYERPOSITIONTOP"
#define MBCCOMP_GET_PLAYERVELOCITYX                "GET_PLAYERVELOCITYX"
#define MBCCOMP_GET_PLAYERVELOCITYY                "GET_PLAYERVELOCITYY"
#define MBCCOMP_GET_PLAYERDIRECTION                "GET_PLAYERDIRECTION"
#define MBCCOMP_GET_PLAYERHEALTH                   "GET_PLAYERHEALTH"
#define MBCCOMP_GET_PLAYERMAXHEALTH                "GET_PLAYERMAXHEALTH"
#define MBCCOMP_GET_PLAYERMOVEMENTTYPE             "GET_PLAYERMOVEMENTTYPE"
#define MBCCOMP_GET_PLAYERISINVULNERABLE           "GET_PLAYERISINVULNERABLE"
#define MBCCOMP_GET_PLAYERISKNOCKEDBACK            "GET_PLAYERISKNOCKEDBACK"
#define MBCCOMP_GET_PLAYERKNOCKBACKSECONDS         "GET_PLAYERKNOCKBACKSECONDS"
#define MBCCOMP_GET_PLAYERKNOCKBACKVELOCITY        "GET_PLAYERKNOCKBACKVELOCITY"
#define MBCCOMP_GET_POSITIONLEFT                   "GET_POSITIONLEFT"
#define MBCCOMP_GET_POSITIONTOP                    "GET_POSITIONTOP"
#define MBCCOMP_GET_VELOCITYX                      "GET_VELOCITYX"
#define MBCCOMP_GET_VELOCITYY                      "GET_VELOCITYY"
#define MBCCOMP_GET_DIRECTION                      "GET_DIRECTION"
#define MBCCOMP_GET_HEALTH                         "GET_HEALTH"
#define MBCCOMP_GET_MAXHEALTH                      "GET_MAXHEALTH"
#define MBCCOMP_GET_MOVEMENTTYPE                   "GET_MOVEMENTTYPE"
#define MBCCOMP_GET_ISINVULNERABLE                 "GET_ISINVULNERABLE"
#define MBCCOMP_GET_ISKNOCKEDBACK                  "GET_ISKNOCKEDBACK"
#define MBCCOMP_GET_KNOCKBACKSECONDS               "GET_KNOCKBACKSECONDS"
#define MBCCOMP_GET_KNOCKBACKVELOCITY              "GET_KNOCKBACKVELOCITY"
#define MBCCOMP_GET_ARENAENTITYCOUNT               "GET_ARENAENTITYCOUNT"
#define MBCCOMP_GET_ARENAENTITYTYPE                "GET_ARENAENTITYTYPE"
#define MBCCOMP_GET_ARENAENTITYPOSITIONLEFT        "GET_ARENAENTITYPOSITIONLEFT"
#define MBCCOMP_GET_ARENAENTITYPOSITIONTOP         "GET_ARENAENTITYPOSITIONTOP"
#define MBCCOMP_GET_ARENAENTITYVELOCITYX           "GET_ARENAENTITYVELOCITYX"
#define MBCCOMP_GET_ARENAENTITYVELOCITYY           "GET_ARENAENTITYVELOCITYY"
#define MBCCOMP_GET_ARENAENTITYDIRECTION           "GET_ARENAENTITYDIRECTION"
#define MBCCOMP_GET_ARENAENTITYHEALTH              "GET_ARENAENTITYHEALTH"
#define MBCCOMP_GET_ARENAENTITYMAXHEALTH           "GET_ARENAENTITYMAXHEALTH"
#define MBCCOMP_GET_ARENAENTITYMOVEMENTTYPE        "GET_ARENAENTITYMOVEMENTTYPE"
#define MBCCOMP_GET_ARENAENTITYDAMAGESECONDS       "GET_ARENAENTITYDAMAGESECONDS"
#define MBCCOMP_GET_ARENAENTITYISINVULNERABLE      "GET_ARENAENTITYISINVULNERABLE"
#define MBCCOMP_GET_ARENAENTITYISKNOCKEDBACK       "GET_ARENAENTITYISKNOCKEDBACK"
#define MBCCOMP_GET_ARENAENTITYKNOCKBACKSECONDS    "GET_ARENAENTITYKNOCKBACKSECONDS"
#define MBCCOMP_GET_ARENAENTITYKNOCKBACKVELOCITY   "GET_ARENAENTITYKNOCKBACKVELOCITY"
#define MBCCOMP_GET_RANDOM                         "GET_RANDOM"

#define MBCCOMP_SET_VELOCITYX             "SET_VELOCITYX"
#define MBCCOMP_SET_VELOCITYY             "SET_VELOCITYY"
#define MBCCOMP_SET_DIRECTION             "SET_DIRECTION"

#define MBCCOMP_DO_SHOOT                  "SHOOT"
#define MBCCOMP_DO_SHOOTTARGET            "SHOOTTARGET"
