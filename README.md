# Compilador de Documentos

Este repositorio contiene un compilador que utiliza una gramática LL(1) para convertir un documento de texto en un archivo PDF utilizando Pandoc.

## Características

- **Gramática LL(1):** La gramática está diseñada para reconocer estructuras específicas en el documento de entrada.
- **Tokens:** Soporte para múltiples tipos de tokens, incluyendo encabezados, texto con estilo, listas, tablas, imágenes, URLs, entre otros.
- **Traducción a Markdown:** Los tokens reconocidos se traducen a formato Markdown.
- **Generación de PDF:** El archivo Markdown se convierte en un archivo PDF utilizando Pandoc.

## Requisitos

- **Visual Studio:** Compilador de C++.
- **Pandoc:** Herramienta de conversión de documentos. Puedes instalar Pandoc siguiendo las instrucciones en [este enlace](https://pandoc.org/installing.html).

## Uso

### Compilar el Compilador

Para compilar el compilador utilizando Visual Studio, sigue estos pasos:

1. Abre Visual Studio.
2. Crea un nuevo proyecto de C++ y agrega el archivo `Compilador.cpp` al proyecto.
3. Compila el proyecto para generar el ejecutable.

### Ejecutar el Compilador

El compilador toma un archivo de texto de entrada (`input.txt`) y genera un archivo Markdown (`output.md`) y un archivo PDF (`generado.pdf`).

Para ejecutar el compilador, sigue estos pasos:

1. Coloca el archivo `input.txt` en el mismo directorio que el ejecutable generado.
2. Ejecuta el ejecutable desde la línea de comandos o desde el entorno de Visual Studio.

### Entrada

El archivo de entrada (`input.txt`) debe contener el texto a procesar. A continuación se muestra un ejemplo de contenido del archivo de entrada:

```
<#-Encabezado Nivel 1-#>
<##-Encabezado Nivel 2-##>
<###-Encabezado Nivel 3-###>
<*-Texto en negrita-*>
</-Texto en cursiva-/>
<_-Texto subrayado-_>
<~-Texto tachado-~>
<{-Encabezado de tabla-}|-Celda de tabla-|&|-Celda de tabla-|>
```

### Combinación de Estilos de Texto

Los estilos de texto pueden ser combinados uno dentro de otro. Por ejemplo, es posible tener un texto en negrita, cursiva y tachado al mismo tiempo. Aquí hay un ejemplo de cómo se puede hacer esto en el archivo de entrada:

```
<*-/-~-Texto en negrita, cursiva y tachado-~-/-*>
```

## Estructura del Repositorio

- `Compilador.cpp`: Archivo principal que contiene el código del compilador.
- `input.txt`: Archivo de entrada con el contenido a procesar.
- `output.md`: Archivo generado en formato Markdown.
- `generado.pdf`: Archivo PDF generado a partir del archivo Markdown.
