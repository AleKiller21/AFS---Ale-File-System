# Proyecto Sistema de Archivos para la clase de Sistemas Operativos II UNITEC#


## Estructuras Sistema de Archivos ##

* SuperBlock = 48 bytes
* DirectoryEntry = 264 bytes
* Inode = 48

**Los tamaños de las estructuras serán un poco mayor al tamaño original que se puede observar con solo ver el struct de cada una, esto por el compilador de C++ usado por VS**

## Comandos ##

* crtdsk <DISK_NAME> <SIZE> <MB/GB> -- Crea un nuevo disco con una partición primaria que tendrá el sistema de archivos montado.
* rename <FILE_NAME> <NEW_NAME> -- Cambia el nombre del archivo especificado por el nombre en el segundo argumento.
* open <DISK_NAME> -- Abre el archivo especificado.
* touch <FILE_NAME> -- Crea un archivo vació.
* import <FILE_PATH> --Importara un archivo del sistema de archivos del sistema operativo al sistema de archivos AFS.
* mount -- Carga las estructuras del sistema de archivos AFS a memoria.
* unmount -- Elimina las estructuras del sistema de archivos AFS de memoria. No se podrá interactuar con ningún archivo hasta que se vuelva a montar.
* ls -- Lista todos los archivos existentes con su respectiva información.
* help -- Muestra todos los comandos disponibles, los argumentos que requieren y una breve descripción del comando.
* cls -- Limpia la pantalla.

## Workflow ##

1. Crear un nuevo disco. Este tendrá el sistema de archivos escrito en el.
2. Abrir el disco.
3. Montar el sistema de archivos. Este comando cargara todas las estructuras a memoria.
4. Interactuar con el sistema de archivos.
5. Cerrar el disco (esto implicitamente ejecutara el comando **unmount**, el cual se puede tambien llamar manualmente).