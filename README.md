# Proyecto Sistema de Archivos para la clase de Sistemas Operativos II UNITEC#

**El sistema de archivos permite solo 85 inodos. Debido a esto solo habrá 85 entradas en el directorio raíz lo cual solo permitirá la creación de 85 archivos.**

## Estructuras Sistema de Archivos ##

* SuperBlock = 48 bytes
* DirectoryEntry = 264 bytes
* Inode = 48

**Los tamaños de las estructuras serán un poco mayor al tamaño original que se puede observar con solo ver el struct de cada una, esto por el compilador de C++ usado por VS**

## Comandos ##

* create_block <DISK_NAME> <SIZE> <MB/GB> -- Crea un nuevo disco con una partición primaria que tendrá el sistema de archivos montado.
* delete_block <DISK_NAME> -- Borrara el disco/particion especificado.
* rename <FILE_NAME> <DELIMITER ';'> <NEW_NAME> -- Cambia el nombre del archivo especificado por el nombre en el segundo argumento. Los nombres pueden llevar espacios.
* open <DISK_NAME> -- Abre el archivo especificado.
* empty <FILE_NAME> -- Crea un archivo vació.
* copy_from_fs <FILE_PATH> <DELIMITER ':'> <TARGET_NAME> --Importara un archivo del sistema de archivos del sistema operativo al sistema de archivos AFS con el nombre especificado. Si el TARGET_NAME esta vacio entonces se importara con el nombre original.
* copy_to_fs <AFS_FILE_NAME> <DELIMITER ';'> <DESTINATION_PATH> -- Exportara el archivo en AFS a la direccion especififcada del FS en el Sistema Operativo.
* mount -- Carga las estructuras del sistema de archivos AFS a memoria.
* unmount -- Elimina las estructuras del sistema de archivos AFS de memoria. No se podrá interactuar con ningún archivo hasta que se vuelva a montar.
* ls -- Lista todos los archivos existentes con su respectiva información.
* help -- Muestra todos los comandos disponibles, los argumentos que requieren y una breve descripción del comando.
* cls -- Limpia la pantalla.
* close -- Cierra el disco abierto. Si el sistema de archivos esta montado, entonces este se desmontara automaticamente.
* exit -- Cierra toda la aplicación.

## Workflow ##

1. Crear un nuevo disco. Este tendrá el sistema de archivos escrito en el.
2. Abrir el disco.
3. Montar el sistema de archivos. Este comando cargara todas las estructuras a memoria.
4. Interactuar con el sistema de archivos.
5. Cerrar el disco (esto implícitamente ejecutara el comando **unmount**, el cual se puede tambien llamar manualmente).
6. Cerrar la aplicación.

La versión más reciente del programa se encuentra en el branch de **develop**. Pronto se pasara a **master** que contiene la versión estable.