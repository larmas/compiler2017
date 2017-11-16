# **TALLER DE DISEÑO DE SOFTWARE 2017** #

>El proyecto de la materia consiste en implementar un compilador para un lenguaje
>imperativo simple, similar a C o Pascal, llamado C-TDS.


### Autores: ###
____
- [Armas Lucas](https://github.com/larmas "Perfil GitHub")
- [Bentolilla Fernando](https://github.com/fernando13 "Perfil GitHub")

### Como correr los tests? ###
____
##### Situado en la raíz del proyecto ejecute los siguientes comandos: #####

* *Compilar el proyecto:*\
 `./script/compile.sh`
* *Si quiere correr casos de test sintácticos:*\
 `./script/runTestSintactico.sh`
* *Si quiere correr casos de test semánticos:*\
 `./script/runTestSemantico.sh`
* *Si quiere correr casos de test para codigo intermedio:*\
 `./script/runTestCodeInter.sh`
* *Si quiere correr casos de test para generacion de codigo assembler:*\
  `./script/runTestAssembly.sh`

### Como compilar un único archivo? ###
____
##### Situado en la raíz del proyecto ejecute el siguiente comando: #####
 `./script/ctds.sh <nombre_del_archivo>`
 
##### IMPORTANTE:
###### Para correr un archivo unico, el mismo debe estar ubicado en la carpeta test/test_assembly y en el comando se debe pasar solo el nombre base del archivo, es decir, para compilar el archivo `/test/test_assembly/add.ctds` usamos el comando `./script/ctds.sh add.ctds`.
###### Luego de compilar un archivo único, tendremos como resultado un archivo que contiene el codigo assembler y un ejecutable, ambos ubicados en la carpeta `/assembly` y con el mismo nombre del archivo que se compiló. Para el ejemplo del punto anterior tendremos: `/assembly/add.s` y `/assembly/add`. 


### Como eliminar archivos generados en la etapa de compilación? ###
____
##### Situado en la raíz del proyecto ejecute el siguiente comando: #####
* `./script/clear.sh`
##### IMPORTANTE:
###### Este comando eliminará todos aquellos archivos con extension `.s` y tambien todos los ejecutables, tanto en el directorio `/src` como en el directorio `/assembly`.
