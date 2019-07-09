# CLI
Command Line Interface for Switch/Router like Machines

# File
cmd.c           : Command Implimentation

cmd.h           : CommandImplimentation

cmd_line.c      : Interface to GNU Read Line

cmd_list.c      : User Command list (This will be filled by user)

cmd_list_cb.c   : User Command list callbacks (This will be filled by user)

cmd_list.h      : User Command list

Makefile        : Makefile

parser.c        : Parser Implimentation

parser.h        : Parser Implimentation

test.c          : Example to test

test.h          : Example to test





# Open Sourse
It's a open source and available for the public to use and modify

# Usage
Just add the command in cmd_list.c and define callback in cmd_list_cb.c
And you are all set to use it for your need

# View
Switch(root)$ enable

Switch(config-enable)> config terminal

Switch(config-term)# ?

        interface  (   ->  ) Config interface
				
        debug      (   ->  ) Set Debug Level
				
        undebug    (   ->  ) Unset Debug Level
				
        show       (  <CR> ) Show Commands
				
        exit       (  <CR> ) Exit to root mode
				
        quit       (  <CR> ) Quit from current mode
				
Switch(config-term)# interface ethernet ?

                   (  <CR> ) If Or If List (1,3-5,7)
									 
Switch(config-term)# interface ethernet  1-4,5-10

Switch(config-if)(e-1-10)# ?

        interface  (   ->  ) Config interface
				
        name       (   ->  ) Set Interface Name
				
        enable     (  <CR> ) Enable Interface
				
        disable    (  <CR> ) Disable Interface
				
        debug      (   ->  ) Set Debug Level
				
        undebug    (   ->  ) Unset Debug Level
				
        show       (  <CR> ) Show Commands
				
        exit       (  <CR> ) Exit to root mode
				
        quit       (  <CR> ) Quit from current mode
				
Switch(config-if)(e-1-10)# sh interfaces br
Port cfg  speed  status   name
---- ---- ------ -------- --------------
1    No   0      unknown
2    No   0      unknown
3    No   0      unknown
4    No   0      unknown
5    No   0      unknown
6    No   0      unknown
7    No   0      unknown
8    No   0      unknown
9    No   0      unknown
10   No   0      unknown

Switch(config-if)(e-1-10)# dis

Switch(config-if)(e-1-10)# sh running-config

!

enable

!

!

config terminal

!

!

interface ethernet 1

    disable

!

!


# Good Things
1. Auto complete the command line
2. support for short hand command (show interface brief -> sh i b)
3. support "?" for help

