# Tarea 0: Rayos Desordenados

Este proyecto es una implementación de una aplicación gráfica en **GTK** que genera y despliega rayos de diferentes longitudes, ángulos y colores, con un origen común en el centro de un círculo. La aplicación permite al usuario configurar el número de rayos (`N`), el número de datos (`k`), y los colores de los rayos.

## Requisitos

Para compilar y ejecutar este proyecto, necesitas las siguientes dependencias:

- **GTK+ 3.0**: Para la interfaz gráfica.
- **Cairo**: Para el dibujo de los rayos y el círculo.
- **GCC**: Compilador de C.
- **pkg-config**: Herramienta para obtener las banderas de compilación y enlazado.
- **Glade**: Para diseñar la interfaz gráfica.
- **GDB** (opcional): Para depuración del código.

## Instalación de Dependencias

### En Ubuntu (o distribuciones basadas en Debian)

Puedes instalar las dependencias una por una con los siguientes comandos:

#### Dependencias básicas

1. **GTK+ 3.0**:

   ```bash
   sudo apt-get install libgtk-3-dev

   ```

2. **Cairo**:

   ```bash
   sudo apt-get install libcairo2-dev

   ```

3. **pkg-config**:

   ```bash
   sudo apt-get install pkg-config

   ```

4. **GCC**:

   ```bash
   sudo apt-get install build-essential

   ```

5. **Glade**:

   ```bash
   sudo apt-get install glade

   ```

6. **GDB**:
   ```bash
   sudo apt-get install gdb
   ```

## Instrucciones de compilación

### Compilación manual

1. Asegúrate de tener instaladas las dependencias (`gtk+-3.0` y `cairo`).
2. Compila el programa usando el siguiente comando:

   ```bash
   make
   ```
