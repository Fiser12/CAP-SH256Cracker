Para compilar este hasher tan chulo (SHA-256), necesitas 'gcc' instalado en tu sistema Unix y ejecutar el siguiente comando en el directorio donde se encuentra este README que estás leyendo:

gcc sha256_hasher.c sha256.c -o Hasher

Lo que acabas de hacer habrá generado un archivo nuevo en el directorio llamado: "Hasher". Ahora estas 'ready' para probar nuestro Hasher.
Para hacer uso del Hasher, solo tienes que ejecutar la siguiente linea en tu terminal.
Ejemplo:

./Hasher kaixo

Y el resultado será el siguiente: 

D050C1CBF2956CDE459DA5C8FD8851DA352C10C52F758D0E3BFBEF2473A802B7 

Si no introduces nada como parametro, entonces no te fallará, pero sí te advertirá de que no has introducido un texto:

"No input text!! :("

Si has llegado hasta aquí, puedes darte por satisfecho! :)))

