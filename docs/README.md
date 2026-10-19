# <img align="left" width="150" height="150" src="https://github.com/ArranzCNL/ImprovedCameraSE-NG/blob/main/assets/Artwork/ic_logo2.png"> ImprovedCameraSE-NG

**Improved Camera SE** es una modificación de cámara en primera persona de tipo inmersivo para
[The Elder Scrolls V: Skyrim Special Edition](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition/).

La versión **NG** hace que esta modificación funcione en múltiples versiones de SkyrimSE, desde la
**1.5.97** hasta la **1.7.104**, gracias a [CommonLibSSE-NG](https://github.com/alandtse/CommonLibSSE-NG)
y a la [Address Library](https://github.com/meh321/AddressLibraryDatabase).

> **Versión de este código:** `ImprovedCameraSE v1.1.2.4228`  
> **Licencia:** Mozilla Public License 2.0 (MPL-2.0)

---

## Índice

- [¿Qué hace este mod?](#qué-hace-este-mod)
- [Características principales](#características-principales)
- [Showcase](#showcase)
- [Requisitos del usuario final](#requisitos-del-usuario-final)
- [Versiones de Skyrim compatibles](#versiones-de-skyrim-compatibles)
- [Instalación](#instalación)
- [Cómo usar el menú](#cómo-usar-el-menú)
- [El concepto de "Fake First Person"](#el-concepto-de-fake-first-person)
- [Compatible con / integraciones](#compatible-con--integraciones)
- [Configuración (INI y perfiles)](#configuración-ini-y-perfiles)
  - [Archivos y rutas](#archivos-y-rutas)
  - [ImprovedCameraSE.ini (`[MODULE DATA]`)](#improvedcameraseini-module-data)
  - [Perfil: `[GENERAL]`](#perfil-general)
  - [Perfil: `[HIDE]`](#perfil-hide)
  - [Perfil: `[FIXES]`](#perfil-fixes)
  - [Perfil: `[RESTRICT ANGLES]`](#perfil-restrict-angles)
  - [Perfil: `[EVENTS]`](#perfil-events)
  - [Perfil: `[FOV]`](#perfil-fov)
  - [Perfil: `[NEARDISTANCE]`](#perfil-neardistance)
  - [Perfil: `[HEADBOB]`](#perfil-headbob)
  - [Perfil: `[CAMERA]`](#perfil-camera)
- [Perfiles (Profiles)](#perfiles-profiles)
- [Registro de errores (log)](#registro-de-errores-log)
- [Problemas conocidos y notas](#problemas-conocidos-y-notas)
- [Compilación desde el código fuente](#compilación-desde-el-código-fuente)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Créditos](#créditos)
- [Licencia](#licencia)

---

## ¿Qué hace este mod?

El juego original oculta el cuerpo del jugador cuando juegas en primera persona y te obliga a pasar
a tercera persona en multitud de situaciones (montar a caballo, montar un dragón, sentarte, fabricar
objetos, ejecuciones, transformaciones en hombre lobo / señor vampiro, cinemáticas, etc.).

**Improved Camera SE** toma el control de la cámara del motor para que **sigas viendo en primera
persona** en esas situaciones, mostrando tu **cuerpo visible** (incluidas manos, armas, sombras y,
de forma opcional, la cabeza). El resultado es una experiencia en primera persona mucho más
inmersiva y coherente, sin cambiar la forma de jugar.

También permite ajustar con precisión el **FOV** (campo de visión), la **distancia cercana**
(_near distance_), la **posición de la cámara** y el **movimiento de cabeza** (_head bob_) para cada
situación por separado, además de **restringir los ángulos** de giro en determinados estados.

## Características principales

- **Cuerpo visible en primera persona** (con opción de mostrarlo también con la consola abierta).
- **Sombras del jugador** en primera persona.
- **Primera persona en eventos** en los que el juego fuerza la tercera persona:
  - Combate, muebles, fabricación (_crafting_), ejecuciones (_killmoves_), _ragdoll_, muerte, canibalismo,
    caballo (incluido combate y transiciones), dragón (incluido combate y transiciones), hombre lobo,
    señor vampiro, _NecroLich_, transformaciones, secuencias con guion (_scripted_) y tercera persona.
- **Cabeza visible** configurable por contexto (normal, combate, caballo, dragón, señor vampiro,
  hombre lobo y secuencias con guion).
- **Brazos en tercera persona** cuando estás en primera persona (con subopciones para antorcha, escudo,
  arco y ballesta, además de sus variantes de bloqueo/apuntado).
- **Ocultar cuerpo** en ciertas acciones (sentarse, dormir, saltar, nadar, rodada en sigilo, atacar,
  ataque fuerte, atacar con arco, ejecución) y **ocultar armas** (arma a dos manos, arco, carcaj).
- **Head bob** (movimiento de cabeza) con intensidad configurable por estado de movimiento.
- **FOV por estado**, **near distance por estado** y **posición de cámara por estado**.
- **Restricción de ángulos** al mirar sentado, montado, volando, en transformaciones y en secuencias
  con guion.
- **Soporte de tacones altos** (_high heels_) y **escala del jugador** según la raza.
- **Detección automática** de otros mods para ajustar el comportamiento (ver más abajo).
- **Menú interno** (Dear ImGui) para editar todo en caliente, con sistema de **perfiles**.

## Showcase

[![Improved Camera SE](https://img.youtube.com/vi/SGQNoSJBfSk/maxresdefault.jpg)](https://www.youtube.com/watch?v=SGQNoSJBfSk "Improved Camera SE")

Créditos del vídeo: [Capt. Panda](https://www.youtube.com/@CaptPanda)

## Requisitos del usuario final

- [Skyrim Script Extender (SKSE64)](https://skse.silverlock.org/) — **mínimo v0.2.0.18** (por la API de _trampolines_).
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444).

> El mod es un plugin SKSE (`ImprovedCameraSE.dll`). No requiere ninguna _esp_ adicional.

## Versiones de Skyrim compatibles

El plugin arranca en cualquier versión de SkyrimSE entre **1.5.97** y **1.7.104**, y detecta la
versión en tiempo de ejecución. Se reconocen explícitamente estos _builds_:

| Build     | Versión de SkyrimSE |
| --------- | ------------------- |
| `k15970`  | 1.5.97              |
| `k16353`  | 1.6.317 – 1.6.353   |
| `k16640`  | 1.6.629 – 1.6.640   |
| `k161130` | 1.6.1130 – 1.6.1170 |
| `k16659`  | 1.6.659 (GOG)       |
| `k161179` | 1.6.1179 (GOG)      |
| `k1799`   | 1.7.99              |
| `k17104`  | 1.7.104 y posteriores |

Las versiones GOG se detectan y añaden `" GOG"` al nombre de la ventana automáticamente.

> **Nota sobre 1.7.x:** dos _hooks_ (`TESObjectCell::Get3D` y `SmoothAnimationTransitions`) apuntan a
> instrucciones situadas **dentro** de sus funciones. La 1.7.x reorganizó ambas, así que el plugin usa
> desplazamientos distintos según el _build_ (`0x7A` / `0x303` en 1.7.x frente a `0x7B` / `0x2F4` en
> 1.5.97 y 1.6.x). Es un comportamiento esperado, no un error de configuración.

## Instalación

1. Instala **SKSE64** y la **Address Library** si aún no lo has hecho.
2. Instala el mod (archivo principal) con tu gestor de mods (Vortex, Mod Organizer 2…) o manualmente.
   El instalador **FOMOD** te pedirá que selecciones tu versión de Skyrim (1.5.97 o 1.6.353+).
3. Al instalar, los archivos quedan colocados así:
   - `Data\SKSE\Plugins\ImprovedCameraSE.dll`
   - `Data\SKSE\Plugins\ImprovedCameraSE\ImprovedCameraSE.ini`
   - `Data\SKSE\Plugins\ImprovedCameraSE\Profiles\Default.ini`
   - `Data\SKSE\Plugins\ImprovedCameraSE\Fonts\…`
4. Ejecuta el juego a través de `skse64_loader.exe`.

Si el plugin no encuentra `skse64_loader.exe` o la versión del juego no está dentro del rango
soportado, mostrará un mensaje de error y se desactivará.

## Cómo usar el menú

- **Abrir el menú:** mantén pulsado **Shift Izquierdo** y pulsa la tecla configurada (`MenuKey`).
  Por defecto es **Inicio / Home** (`0x24`).
- **Cerrar el menú:** **Escape** (o cuando la ventana pierde el foco).
- **Guardar perfil:** **Ctrl + S**.
- En los deslizadores puedes **hacer Ctrl + clic** para escribir un valor numérico exacto.

Dentro del menú encontrarás estas pestañas/vistas: **General**, **Hide**, **RestrictAngles**,
**Events**, **FOV**, **NearDistance**, **Headbob**, **Camera**, además de la ventana **Module Data**
(solo lectura) y las herramientas de depuración de ImGui. El propio menú permite también
**Guardar / Cambiar / Borrar perfiles**.

## El concepto de "Fake First Person"

Es la característica clave del mod y conviene entenderla:

En algunas situaciones el motor de Skyrim **no puede** ponerse realmente en primera persona (montar,
dragón, muebles, transiciones, secuencias con guion…). En lugar de forzar el estado del motor, el mod
**toma la cámara de tercera persona existente, la lleva hasta el nodo de la cabeza del jugador y
oculta/ajusta el cuerpo** para que la vista _parezca_ primera persona. A eso se le llama
**"fake first person"** (falsa primera persona).

Consecuencias prácticas:

- Sigues viendo en primera persona aunque el motor crea que estás en tercera.
- La rueda del ratón puede usarse para acercar/alejar en esos estados; al hacer _zoom_ completo se
  fija la falsa primera persona.
- Mientras la falsa primera persona está activa, el mod **pide el control de la cámara a SmoothCam**
  y **desactiva momentáneamente el _headtracking_ y el movimiento direccional de True Directional
  Movement** para evitar conflictos, restaurándolos al salir.

## Compatible con / integraciones

El mod detecta automáticamente varios mods y adapta su comportamiento:

| Mod                                    | Tipo de integración                                                                                       |
| -------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| **SmoothCam**                          | API (v3). Solicita/libera el control de la cámara cuando la falsa primera persona está activa.            |
| **True Directional Movement (TDM)**    | API (v2 by _Ershin_). Desactiva _headtracking_ y modo direccional durante la falsa primera persona.       |
| **Archery Gameplay Overhaul (AGO)**    | Detección por `DSerArcheryGameplayOverhaul.esp` y activación de parche.                                   |
| **Immersive Equipment Displays (IED)** | Detección por DLL; si está presente, avisa de que uses IED para ocultar armas y desactiva esas opciones.  |
| **Show Player In Menus (SPIM)**        | Detección por _pattern scan_ para resetear los brazos al abrir inventario/magia.                          |
| **OStim**                              | Si `OStim.dll` está cargado, se omite el parche de desvanecido de tercera persona.                        |
| **CaptureWarmer**                      | Si `CaptureWarmer.dll` está cargado, se instala un parche de desvanecido (_fade_) extra.                  |
| **Paragliding**                        | Detectado por la animación `ParaGlide`; se fuerza a tercera persona con soporte de falsa primera persona. |
| **Touring Carriages**                  | Detectado por la animación `CartPrisonerCSway`.                                                           |
| **CFPAO / Animated Interactions**      | Manejo de la animación `TakeItem` para los brazos.                                                        |
| **Ultimate Animated Potions**          | Manejo de la animación `DrinkPotion`.                                                                     |
| **High Heels**                         | Soporte mediante el _offset_ de tacones.                                                                  |
| **ReShade**                            | Comprobación opcional de compatibilidad (con `CheckCompatibility=1`).                                     |
| **Skyrim Together**                    | Comprobación opcional; reconfigura el menú a modo _Overlay_ (con `CheckCompatibility=1`).                 |

> `CheckCompatibility` está **desactivado por defecto** (por una consideración relativa a Vortex).
> Actívalo solo si usas Skyrim Together.

## Configuración (INI y perfiles)

La configuración se reparte en dos niveles:

1. **`ImprovedCameraSE.ini`** → datos del módulo (nombre de ventana, modo de menú, tecla, fuente y
   **qué perfil** cargar). Se lee primero.
2. **Perfiles** (`Profiles\*.ini`) → todos los ajustes de comportamiento (cuerpo, eventos, FOV,
   posiciones, etc.). El perfil indicado por `ProfileName` se carga automáticamente.

Puedes **editar los INI a mano** (el juego debe estar cerrado) o hacerlo desde el menú _in-game_.
Los valores que no existan o estén mal formados harán que aparezca un `MessageBox` indicando
`Invalid: ImprovedCameraSE.ini` o `Invalid Profile: <nombre>`.

> **Nota:** la sección `[FIXES]` existe en el INI, pero en el menú sus dos opciones aparecen dentro de
> la pestaña **General** (se eliminó la pestaña "Fixes" en v1.1.0).

### Archivos y rutas

| Archivo                      | Ruta                                                                  |
| ---------------------------- | --------------------------------------------------------------------- |
| Plugin                       | `Data\SKSE\Plugins\ImprovedCameraSE.dll`                              |
| Config principal             | `Data\SKSE\Plugins\ImprovedCameraSE\ImprovedCameraSE.ini`             |
| Perfiles                     | `Data\SKSE\Plugins\ImprovedCameraSE\Profiles\<nombre>.ini`            |
| Fuentes                      | `Data\SKSE\Plugins\ImprovedCameraSE\Fonts\`                           |
| Disposición del menú (ImGui) | `Data\SKSE\Plugins\ImprovedCameraSE\editorconfig.ini`                 |
| Log                          | `Documents\My Games\Skyrim Special Edition\SKSE\ImprovedCameraSE.log` |

### ImprovedCameraSE.ini (`[MODULE DATA]`)

| Clave                | Descripción                                                                                  | Valor por defecto               |
| -------------------- | -------------------------------------------------------------------------------------------- | ------------------------------- |
| `FileName`           | Nombre del ejecutable del juego.                                                             | `SkyrimSE.exe`                  |
| `FileVersionMin`     | Versión mínima soportada.                                                                    | `1.5.97.0`                      |
| `FileVersionMax`     | Versión máxima soportada.                                                                    | `1.7.104.0`                     |
| `WindowName`         | Nombre de la ventana de la aplicación.                                                       | `Skyrim Special Edition`        |
| `CheckCompatibility` | Comprobaciones de compatibilidad (Skyrim Together, ReShade). Desactivado por Vortex.         | `0`                             |
| `MenuMode`           | Estilo del menú: `0` Desactivado, `1` Externo (ventana superpuesta), `2` Interno (embebido). | `2`                             |
| `MenuTimeout`        | Segundos máximos buscando la ventana del juego.                                              | `120`                           |
| `MenuKey`            | Tecla para abrir el menú (hex). `0x24` = Inicio. F1–F12 = `0x70`–`0x7B`.                     | `0x24`                          |
| `MenuFont`           | Fuente personalizada (relativa a `Fonts\` o de `C:\Windows\Fonts\`).                         | `OpenSans\OpenSans-Regular.ttf` |
| `MenuFontSize`       | Tamaño de la fuente del menú.                                                                | `18.0`                          |
| `ProfileName`        | Perfil a cargar.                                                                             | `Default.ini`                   |

> Para abrir el menú **siempre** hay que mantener pulsado **Shift Izquierdo** antes de pulsar `MenuKey`.

### Perfil: `[GENERAL]`

| Clave                           | Descripción                                                              |
| ------------------------------- | ------------------------------------------------------------------------ |
| `bEnableBody`                   | Cuerpo visible en primera persona.                                       |
| `bEnableBodyConsole`            | Cuerpo visible con la consola abierta.                                   |
| `bEnableShadows`                | Sombras del jugador en primera persona.                                  |
| `bAdjustPlayerScale`            | Ajusta la altura de la cámara a la escala de la raza.                    |
| `fBodyHeightOffset`             | Desplaza el cuerpo para ajustar la altura de la cámara.                  |
| `bEnableHead`                   | Muestra el nodo de la cabeza en primera persona (puede mostrar el pelo). |
| `bEnableHeadCombat`             | Cabeza visible en combate.                                               |
| `bEnableHeadHorse`              | Cabeza visible a caballo.                                                |
| `bEnableHeadDragon`             | Cabeza visible montando dragón.                                          |
| `bEnableHeadVampireLord`        | Cabeza visible como señor vampiro.                                       |
| `bEnableHeadWerewolf`           | Cabeza visible como hombre lobo.                                         |
| `bEnableHeadScripted`           | Cabeza visible en secuencias con guion.                                  |
| `bEnableThirdPersonArms`        | Usa brazos de tercera persona en primera persona.                        |
| `bEnableThirdPersonTorch`       | Brazos de tercera persona con antorcha.                                  |
| `bEnableThirdPersonTorchBlock`  | Ídem con bloqueo de antorcha.                                            |
| `bEnableThirdPersonShield`      | Brazos de tercera persona con escudo.                                    |
| `bEnableThirdPersonShieldBlock` | Ídem con bloqueo de escudo.                                              |
| `bEnableThirdPersonBow`         | Brazos de tercera persona con arco.                                      |
| `bEnableThirdPersonBowAim`      | Ídem apuntando con arco.                                                 |
| `bEnableThirdPersonCrossbow`    | Brazos de tercera persona con ballesta.                                  |
| `bEnableThirdPersonCrossbowAim` | Ídem apuntando con ballesta.                                             |

### Perfil: `[HIDE]`

| Clave          | Descripción                                                                   |
| -------------- | ----------------------------------------------------------------------------- |
| `b2HWeapon`    | Oculta el arma a dos manos en la espalda en primera persona (evita recortes). |
| `bBow`         | Oculta el arco.                                                               |
| `bQuiver`      | Oculta el carcaj.                                                             |
| `bSitting`     | Oculta el cuerpo al sentarse.                                                 |
| `bSleeping`    | Oculta el cuerpo al dormir.                                                   |
| `bJumping`     | Oculta el cuerpo al saltar.                                                   |
| `bSwimming`    | Oculta el cuerpo al nadar.                                                    |
| `bSneakRoll`   | Oculta el cuerpo en la rodada en sigilo.                                      |
| `bAttack`      | Oculta el cuerpo al atacar.                                                   |
| `bPowerAttack` | Oculta el cuerpo al atacar fuerte.                                            |
| `bAttackBow`   | Oculta el cuerpo al disparar con arco.                                        |
| `bKillmove`    | Oculta el cuerpo en ejecuciones.                                              |

> Si **Immersive Equipment Displays** está instalado, las opciones de ocultar armas se desactivan y se
> recomienda usar IED en su lugar.

### Perfil: `[FIXES]`

| Clave                            | Descripción                                                                                                                                                      |
| -------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `bFirstPersonOverhaul`           | Corrige/activa mods de _First Person Overhaul_.                                                                                                                  |
| `bOverrideVanillaArmsOnMovement` | Evita que se oculten los brazos al moverse (esprint, nadar, salto) con `bFirstPersonOverhaul=1`. Úsalo si **no** instalaste las animaciones de esprint de CFPAO. |

### Perfil: `[RESTRICT ANGLES]`

Limita cuánto puedes girar la vista en cada situación (valores en grados).

| Clave                    | Descripción                                                                 | Defecto |
| ------------------------ | --------------------------------------------------------------------------- | ------- |
| `fSitting`               | Ángulo máx. izquierda/derecha sentado (0–90).                               | `80`    |
| `fSittingMaxLookingUp`   | Ángulo máx. hacia arriba sentado (0–90).                                    | `45`    |
| `fSittingMaxLookingDown` | Ángulo máx. hacia abajo sentado (0–90).                                     | `70`    |
| `fMounted`               | Ángulo máx. izquierda/derecha a caballo (0–90).                             | `70`    |
| `fMountedMaxLookingUp`   | Ángulo máx. arriba en combate montado (0–90).                               | `60`    |
| `fMountedMaxLookingDown` | Ángulo máx. abajo en combate montado (0–90).                                | `60`    |
| `fFlying`                | Ángulo máx. izquierda/derecha montando dragón (0–90).                       | `70`    |
| `fFlyingMaxLookingUp`    | Ángulo máx. arriba montando dragón (0–90).                                  | `70`    |
| `fFlyingMaxLookingDown`  | Ángulo máx. abajo montando dragón (0–90).                                   | `70`    |
| `fVampireLord`           | Ángulo máx. izquierda/derecha como señor vampiro (0–180).                   | `80`    |
| `fWerewolf`              | Ángulo máx. izquierda/derecha como hombre lobo (0–180).                     | `80`    |
| `fNecroLich`             | Ángulo máx. izquierda/derecha como _NecroLich_ (0–180).                     | `80`    |
| `fScripted`              | Ángulo máx. izquierda/derecha en tercera persona forzada con guion (0–180). | `65`    |
| `fScriptedPitch`         | Ángulo máx. arriba/abajo en tercera persona forzada con guion (0–90).       | `65`    |

### Perfil: `[EVENTS]`

Activa (`1`) o desactiva (`0`) el mantener la primera persona (o falsa primera persona) en cada
situación. Todos por defecto a `1`.

| Clave                | Situación                              |
| -------------------- | -------------------------------------- |
| `bFirstPerson`       | Primera persona normal.                |
| `bFirstPersonCombat` | Primera persona con arma desenvainada. |
| `bFurniture`         | Muebles (sentarse, etc.).              |
| `bCrafting`          | Fabricación (forja, yunque, etc.).     |
| `bKillmove`          | Ejecuciones.                           |
| `bRagdoll`           | _Ragdoll_ (quedar tendido).            |
| `bDeath`             | Muerte.                                |
| `bCannibal`          | Canibalismo.                           |
| `bHorse`             | A caballo.                             |
| `bHorseCombat`       | Combate a caballo.                     |
| `bHorseTransition`   | Transición de montar/desmontar.        |
| `bDragon`            | Montando dragón.                       |
| `bDragonCombat`      | Combate montando dragón.               |
| `bDragonTransition`  | Transición de montar/desmontar dragón. |
| `bVampireLord`       | Señor vampiro.                         |
| `bWerewolf`          | Hombre lobo.                           |
| `bTransform`         | Transformaciones.                      |
| `bNecroLich`         | _NecroLich_.                           |
| `bScripted`          | Secuencias con guion.                  |
| `bThirdPerson`       | Tercera persona.                       |

### Perfil: `[FOV]`

`bEnableOverride` (`1`/`0`) activa la sobrescritura de FOV. A partir de ahí, cada valor ajusta el
campo de visión de ese estado. Defecto: `80` en todos salvo `fFirstPersonHands` (`65`).

Claves: `fFirstPerson`, `fFirstPersonHands`, `fFirstPersonCombat`, `fFurniture`, `fCrafting`,
`fKillmove`, `fRagdoll`, `fDeath`, `fCannibal`, `fHorse`, `fHorseCombat`, `fHorseTransition`,
`fDragon`, `fDragonCombat`, `fDragonTransition`, `fVampireLord`, `fWerewolf`, `fNecroLich`,
`fScripted`, `fThirdPerson`.

### Perfil: `[NEARDISTANCE]`

`bEnableOverride` (`1`/`0`) activa la sobrescritura de la distancia cercana (el plano de recorte
próximo de la cámara). Valores más bajos reducen el recorte del cuerpo/armas al mirar hacia abajo,
pero pueden provocar artefactos.

| Clave                 | Descripción                                                                  | Defecto |
| --------------------- | ---------------------------------------------------------------------------- | ------- |
| `fFirstPersonDefault` | Valor por defecto en primera persona (funciona fuera del umbral de _pitch_). | `15`    |
| `fPitchThreshold`     | Umbral de inclinación que dispara `fFirstPerson`.                            | `-69`   |
| `fFirstPerson`        | Valor reducido al alcanzar el umbral.                                        | `4`     |
| `fFirstPersonCombat`  | Primera persona en combate.                                                  | `15`    |
| `fSitting`            | Sentado / durmiendo.                                                         | `10`    |
| `fFurniture`          | Muebles.                                                                     | `4`     |
| `fCrafting`           | Fabricación.                                                                 | `4`     |
| `fKillmove`           | Ejecuciones.                                                                 | `4`     |
| `fRagdoll`            | _Ragdoll_.                                                                   | `4`     |
| `fDeath`              | Muerte.                                                                      | `4`     |
| `fCannibal`           | Canibalismo.                                                                 | `4`     |
| `fHorse`              | A caballo.                                                                   | `13`    |
| `fHorseCombat`        | Combate a caballo.                                                           | `6`     |
| `fHorseTransition`    | Transición de caballo.                                                       | `2`     |
| `fDragon`             | Dragón.                                                                      | `15`    |
| `fDragonCombat`       | Combate con dragón.                                                          | `15`    |
| `fDragonTransition`   | Transición de dragón.                                                        | `2`     |
| `fVampireLord`        | Señor vampiro.                                                               | `15`    |
| `fWerewolf`           | Hombre lobo.                                                                 | `15`    |
| `fNecroLich`          | _NecroLich_.                                                                 | `15`    |
| `fScripted`           | Secuencias con guion.                                                        | `2`     |
| `fThirdPerson`        | Tercera persona.                                                             | `15`    |

### Perfil: `[HEADBOB]`

Los ajustes `b*` (0/1) activan el _head bob_ por estado de movimiento. Los `fRotation*` (0.0–1.0)
controlan cuánto rota la cámara con la cabeza. En el menú hay un botón **Sync Sliders** para mover
todos los deslizadores de rotación a la vez (tomando el valor de _Idle_).

- `bIdle`, `bWalk`, `bRun`, `bSprint`, `bCombat`, `bSneak`, `bSneakRoll`
- `fRotationIdle`, `fRotationWalk`, `fRotationRun`, `fRotationSprint`, `fRotationCombat`,
  `fRotationSneak`, `fRotationSneakRoll`

> El _head bob_ de combate está desactivado por ahora (puede desalinear el nodo de las armas).
> Todos los valores por defecto son `0`.

### Perfil: `[CAMERA]`

Desplazamiento de la cámara respecto al nodo de la cabeza (X/Y/Z) para cada estado. Por defecto,
`fFirstPersonPosY=8`, `fHorsePosY=19`, `fDragonPosY=19`/`fDragonPosZ=5`, `fNecroLichPosX=2`/`Y=27`/`Z=15`,
`fScriptedPosY=13`, `fVampireLordPosZ=8`/`fHorseCombatPosZ=8`, y el resto a `0`.

Claves: `fFirstPersonPos*`, `fFirstPersonCombatPos*`, `fHorsePos*`, `fHorseCombatPos*`,
`fDragonPos*`, `fVampireLordPos*`, `fWerewolfPos*`, `fNecroLichPos*`, `fScriptedPos*` (cada una con
`X`, `Y`, `Z`).

## Perfiles (Profiles)

- Los perfiles son archivos `Profiles\<nombre>.ini` con todos los ajustes anteriores.
- El perfil cargado se indica con `ProfileName` en `ImprovedCameraSE.ini`.
- Desde el menú: **File → Profiles** para cambiar, **Save Profile** (Ctrl+S) para guardar, y
  **Delete Profile** para borrar (no se puede borrar `Default.ini`).
- Al guardar sobre `Default` se te pedirá un nombre nuevo; si estás en otro perfil, se sobrescribe.
- El perfil `Default.ini` viene incluido como referencia de valores recomendados.

## Registro de errores (log)

- Se escribe en: `Documents\My Games\Skyrim Special Edition\SKSE\ImprovedCameraSE.log`.
- Se **trunca en cada arranque**.
- Nivel de registro: `trace` (se guarda todo), con marca de tiempo y versión del plugin.
- El log es también el destino de los avisos internos de CommonLibSSE (carga de la Address
  Library, estadísticas del _trampoline_, comprobaciones de los puntos de parcheo), porque el
  _logger_ del plugin se registra como `spdlog` por defecto.
- En compilaciones **Debug** también se abre una consola y se imprime por salida estándar.
- Si el juego se cierra de golpe, se genera además `ImprovedCameraSE.crash.log` (mismo directorio)
  con el código de excepción, la dirección que falló (`módulo+0xRVA`), el tipo de violación de
  acceso, los registros de la CPU y las direcciones de retorno de la pila que caen dentro del DLL.
  Sirve para localizar el fallo sin depender de Crash Logger.

## Problemas conocidos y notas

- **Tacones altos:** hay un bug al cambiar de una partida con personaje femenino a una con personaje
  masculino, que puede dejar la cámara más baja.
- **Pestaña Fixes:** eliminada en v1.1.0; sus opciones pasaron a **General**.
- **Head bob de combate:** desactivado temporalmente.
- **Skyrim Together:** si lo usas, activa `CheckCompatibility=1` y usa el modo de menú _Externo/Overlay_.
- **Menú en pantalla completa:** el modo `MenuMode=1` (externo) no se recomienda con _Fullscreen_;
  usa `MenuMode=2` (interno).
- **`fControllerBufferDepth`:** eliminado del INI; si lo necesitas, ajústalo en tu `skyrim.ini`.
- **Ocultar armas:** si usas IED, hazlo desde IED y no desde el mod.
- Si la cámara se queda en un estado inconsistente, usa **Help → Reset State** en el menú.

## Compilación desde el código fuente

La compilación verificada se hace con **CMake + Ninja**, usando `clang-cl`/`lld-link` y un
_sysroot_ de Windows obtenido con [`xwin`](https://github.com/Jake-Shadle/xwin). Esto permite
compilar el DLL **desde Linux** sin Visual Studio; el resultado es un PE de Windows idéntico en
funcionamiento al que produciría MSVC.

> La solución de Visual Studio (`build/VC17/ImprovedCameraSE_VS2022.sln`) sigue en el repositorio
> como referencia histórica, pero **ya no es el sistema de compilación del proyecto** y no se usa
> para las compilaciones actuales.

### Requisitos de compilación

- **CMake** ≥ 3.24 y **Ninja**.
- **LLVM/Clang** con `clang-cl`, `lld-link` y `llvm-rc` (el proyecto se compila con `clang-cl`).
- Un **_sysroot_ de Windows** generado con `xwin` (MSVC 14.44 + Windows SDK 10.0.26100).
- **vcpkg** para las dependencias (`spdlog`, `fmt`, `directxmath`…).
- **Git** con submódulos.

Todo se orquesta con los ficheros de build del repositorio:

| Fichero                                        | Función                                                     |
| ---------------------------------------------- | ----------------------------------------------------------- |
| `CMakeLists.txt`                               | Proyecto principal (DLL + `imgui` + `minhook` + CommonLibSSE-NG) |
| `CMakePresets.json`                            | Preset `linux-clangcl` (Release, `x64-windows-static-clangcl`)   |
| `cmake/toolchain-linux-clangcl-static.cmake`   | Toolchain de `clang-cl` + `lld-link` + sysroot               |
| `cmake/triplets/x64-windows-static-clangcl.cmake` | _Triplet_ de vcpkg con **CRT estática**                  |
| `vcpkg.json`                                   | Manifiesto de dependencias                                   |

> Se usa **CRT estática** (`MultiThreaded`) a propósito: así el DLL no depende de la
> `MSVCP140.dll`/`VCRUNTIME140.dll` que trae el _prefix_ de Wine, a menudo demasiado antiguas.

### Submódulos (clonar de forma recursiva)

El proyecto usa submódulos de Git. Clona con `--recursive` o inicialízalos después:

```bash
git clone --recursive https://github.com/ArranzCNL/ImprovedCameraSE-NG.git
# o, si ya lo clonaste:
git submodule update --init --recursive
```

| Submódulo         | URL                                          | Rama/ref  |
| ----------------- | -------------------------------------------- | --------- |
| `CommonLibSSE-NG` | https://github.com/alandtse/CommonLibSSE-NG  | `v12.1.0` |
| `mINI`            | https://github.com/ArranzCNL/mINI            | —         |
| `minhook`         | https://github.com/ArranzCNL/minhook         | `dev`     |
| `imgui`           | https://github.com/ArranzCNL/imgui           | `v1.89.9` |
| `glm`             | https://github.com/g-truc/glm                | —         |

`stb` va incluido en el árbol (`vendor/stb`), y `CommonLibSSE-NG` aporta `spdlog`, `rapidcsv` y `xbyak`.

> **Sobre `CommonLibSSE-NG`:** se usa el _fork_ mantenido por
> [alandtse](https://github.com/alandtse/CommonLibSSE-NG), fijado en la etiqueta **`v12.1.0`**.
> El _fork_ original de `ArranzCNL` (`flatrim`) está archivado (solo lectura desde agosto de 2025) y
> **no** sabe leer la Address Library en formato 5 que usan SkyrimSE 1.7.x. De ahí que el plugin
> dejara de cargar con _«must be recompiled for new address library»_.

### Detalles técnicos de la compilación

- **Proyecto principal:** `ImprovedCameraSE` (DLL), **C++20** / **C17**, `MultiByte`.
- **Subproyectos:** `imgui` y `minhook` (como bibliotecas estáticas) más `CommonLibSSE` (vía
  `add_subdirectory`).
- **Macros de compilación:** `ENABLE_SKYRIM_SE=1`, `ENABLE_SKYRIM_AE=1` (build **flatrim**, que
  empaqueta los caminos de SE y AE en un único DLL y elige uno u otro en tiempo de ejecución),
  `SKSE_SUPPORT_XBYAK=1` y `STBI_NO_SIMD` (evita el choque entre el `__cpuid` de `stb_image` y el
  que inyecta `xbyak`).
- **Salida:** `build/linux-clangcl/bin/ImprovedCameraSE.dll` junto a la carpeta de recursos
  `ImprovedCameraSE/` (que se copia automáticamente en el paso _post-build_).
- **Independencia de versión:** el DLL se marca con `AddressLibraryV5 | NoStructUse` y
  `AddressLibraryPostAE`, que es justo lo que exige SKSE en SkyrimSE 1.7.104.
- **_Trampoline_:** `SKSE::Init` reserva el pool completo (`trampoline = true`, `0x1000` bytes).
  En CommonLibSSE-NG esa reserva está protegida por `std::call_once`, así que si la hiciera el
  primer _hook_ (14 bytes) el resto fallaría con
  _«Failed to handle allocation request»_.
- **No hay CI/workflows** en el repositorio; la compilación es manual.
- **Selector de mano en `GetEquippedItemTypeID`:** el cuarto argumento de la función del juego
  (mano izquierda/derecha) se pasa como `std::int32_t` (`rightHand ? 1 : 0`). Declararlo como `bool`
  permitía que Clang reenviara los 24 bits altos sin definir del registro (`mov bl, dl` seguido de
  `mov r9d, ebx`); Skyrim los interpretaba como una mano inválida y tomaba la rama que
  desreferencia el primer argumento (que se pasa a propósito como `nullptr`), provocando un
  `EXCEPTION_ACCESS_VIOLATION` en `SkyrimSE.exe+0xA00980` justo después de cargar la partida.

### Pasos

1. Instala CMake, Ninja, LLVM/Clang, `xwin` y `vcpkg` (o usa un entorno ya preparado que los
   exponga en el `PATH`).
2. Genera el _sysroot_ de Windows con `xwin` y exporta `XWIN_SYSROOT` (o el ajuste equivalente del
   _toolchain_).
3. Clona el repositorio con submódulos (`--recursive`).
4. Configura con el preset incluido:

   ```bash
   cmake --preset linux-clangcl
   ```

5. Compila:

   ```bash
   cmake --build build/linux-clangcl -j$(nproc)
   ```

6. Copia `ImprovedCameraSE.dll` y la carpeta `ImprovedCameraSE/` desde
   `build/linux-clangcl/bin/` al `Data\SKSE\Plugins\` de tu instalación de SkyrimSE
   (en MO2, dentro de la carpeta del mod correspondiente).

## Estructura del proyecto

```
ImprovedCameraSE-NG/
├── assets/                      # Recursos de ejecución y del instalador
│   ├── Artwork/                 # Logos
│   ├── Fomod/                   # Metadatos e instalador FOMOD
│   └── ImprovedCameraSE/        # INI por defecto, perfiles y fuentes
├── build/
│   ├── VC17/                    # Solución de Visual Studio 2022 (histórica)
│   └── linux-clangcl/           # Compilación CMake (clang-cl, verificada)
├── cmake/
│   ├── toolchain-linux-clangcl-static.cmake   # Toolchain clang-cl + lld-link + xwin
│   └── triplets/x64-windows-static-clangcl.cmake  # Triplet de vcpkg (CRT estática)
├── CMakeLists.txt               # Proyecto principal
├── CMakePresets.json            # Preset linux-clangcl
├── vcpkg.json                   # Manifiesto de dependencias
├── docs/                        # README, CHANGELOG y AUTHORS
└── ImprovedCamera/
    ├── include/                 # Cabeceras (api, cameras, menu, settings, systems, utils…)
    ├── source/                  # Implementación (cameras, menu, skyrimse, systems…)
    ├── PCH/                     # Cabecera precompilada
    ├── resource/                # Recursos del DLL (iconos, versión)
    └── vendor/                  # Dependencias (submódulos)
```

**Arquitectura (resumen):**

- `systems/` → configuración (mINI), registro (spdlog), menú (Dear ImGui), ventana y gráficos (D3D11).
- `skyrimse/` → ganchos (_hooks_ con MinHook + trampolines/vtables), direcciones (_Address Library_),
  observadores de eventos del juego y el orquestador `ImprovedCameraSE`.
- `cameras/` → un `ICamera` por familia de cámara (FirstPerson, ThirdPerson, Horse, Dragon, Furniture,
  Ragdoll, DeathCinematic, Transition) con sus subestados.
- `settings/` → estructuras de ajustes (`ModuleData`, `General`, `Hide`, `Fixes`, `RestrictAngles`,
  `Events`, `FOV`, `NearDistance`, `Headbob`, `Camera`, `Logging`).

## Créditos

- **LogicDragon** por [Enhanced Camera](https://www.nexusmods.com/skyrim/mods/57859/).
- **inmundano** por el grueso del trabajo de ingeniería inversa de Improved Camera SE.
- [Ian Patterson](https://github.com/ianpatt/skse64) y los desarrolladores de SKSE64.
- [Ryan-rsm-Mckenzie](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE) (CommonLibSSE original),
  [powerof3](https://github.com/powerof3/CommonLibSSE) (versión de la comunidad),
  [CharmedBaryon](https://github.com/CharmedBaryon/CommonLibSSE-NG) (versión nextgen) y
  [alandtse](https://github.com/alandtse/CommonLibSSE-NG) (mantenimiento del _fork_ usado aquí),
  junto al resto de colaboradores de CommonLibSSE.
- [meh321](https://github.com/meh321) por la Address Library.

**Mantenedor:** [ArranzCNL](https://github.com/ArranzCNL)
**Colaboradores principales:** [TwistedModding](https://github.com/TwistedModding) (soporte y pruebas),
[Kruziikrel13](https://github.com/Kruziikrel13) (desarrollo), [Tytaen](https://github.com/Tytaen) y
[Jessmari](https://github.com/Jessmari00) (pruebas).
**Autor original:** © 2019 inmundano.
**Colaboradores anteriores:** © 2023 [CJ](https://github.com/BarnJoey) (pruebas y parches de Papyrus),
© 2023 [Lam0rak](https://www.youtube.com/@Lam0rak) (arte), © 2022 [noughtmare](https://github.com/noughtmare) (desarrollo).

Consulta [AUTHORS.md](AUTHORS.md) y [CHANGELOG.md](CHANGELOG.md) para más detalle.

## Licencia

Este proyecto se distribuye bajo la **Mozilla Public License 2.0 (MPL-2.0)**.
Consulta el archivo [LICENSE](../LICENSE) para el texto completo.
