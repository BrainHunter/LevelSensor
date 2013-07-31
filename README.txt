	---- COMPILE: ----

type:
make 


	----  Optional: MAKEFILE Options: ----

For RS232 you can add these Lines to save some space in flash (this is default)
219: PRINTF_LIB = $(PRINTF_LIB_MIN)
231: SCANF_LIB = $(SCANF_LIB_MIN)

For TWI only you can delete the variables: 

219: PRINTF_LIB = 
231: SCANF_LIB = 

this will also save some flash.

	---- Config Variables: ----