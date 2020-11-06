# EDAT
database exercises

(06/11/20) Creación de Patch-1.
  Objetivos:
    1) Implementación correcta de relación entre menús (al terminar una consulta nos mantenemos en el menú desde el que se hizo).
    2) Mejora del intefaz gráfico para facilitar la lectura.
    
  Realizados:
    1) programa.c:21: Nueva macro OUT (definida en menu.h) para indicar la salida (Exit) de los menús.
       menu.c: Estructura if-else al final de cada menú para convertir su Exit particular en la general (OUT).
       
    2) menu.h y consultas.h: Nuevas macros para las strings que se utilizan comunmente ==> Ahora puts(MACRO); en sus .c respectivos.
    
    
    
    
    
    
    
    
    
