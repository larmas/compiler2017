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
* *Si quiere correr un archivo unico:*\
 `./script/ctds.sh <nombre_del_archivo>`

##### IMPORTANTE:
###### Para correr un archivo unico, el mismo debe estar ubicado en la carpeta test/test_codeinter y en el comando se debe pasar el nombre del archivo sin el path.

### Como eliminar archivos generados en la etapa de compilación? ###
____
##### Situado en la raíz del proyecto ejecute el siguiente comando: #####
* `./script/clear.sh`
