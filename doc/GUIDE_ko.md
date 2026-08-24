# Ratpac-two 설치 및 사용 가이드 (macOS / Apple Silicon)

이 문서는 **Apple Silicon Mac에서 이 포크(`ratpac-two_for_RENE_on_mac`)를 빌드하고 사용하는 방법**을 한글로 정리한 것이다.

상위 문서인 [ratpac.readthedocs.io](https://ratpac.readthedocs.io)는 리눅스와 컨테이너 환경을 전제로 하며, macOS에서 실제로 부딪히는 문제들은 다루지 않는다. 이 문서는 그 간극을 메우는 것이 목적이다.

문서의 내용은 다음 환경에서 **실제로 실행해 확인한 것**이다.

| 항목 | 버전 |
|---|---|
| macOS | 15.4.1 (Darwin 24.4.0), arm64 |
| Apple clang | 17.0.0 |
| CMake | 4.4.2 |
| ROOT | 6.40.02 (Homebrew, arm64) |
| Geant4 | 11.4.2 (소스 빌드, arm64) |
| Ratpac-two | 2.1.3 |

---

## 목차

1. [설치](#1-설치)
   - [1.1 사전 준비물](#11-사전-준비물)
   - [1.2 아키텍처 함정 — 가장 먼저 확인할 것](#12-아키텍처-함정--가장-먼저-확인할-것)
   - [1.3 빌드](#13-빌드)
   - [1.4 환경 설정](#14-환경-설정)
   - [1.5 설치 확인](#15-설치-확인)
2. [기본 사용법](#2-기본-사용법)
   - [2.1 rat 실행](#21-rat-실행)
   - [2.2 매크로 파일 구조](#22-매크로-파일-구조)
   - [2.3 검출기 선택](#23-검출기-선택)
   - [2.4 이벤트 생성기](#24-이벤트-생성기)
   - [2.5 프로세서](#25-프로세서)
   - [2.6 출력](#26-출력)
3. [결과 분석](#3-결과-분석)
4. [이 포크의 로컬 수정 사항](#4-이-포크의-로컬-수정-사항)
5. [트러블슈팅](#5-트러블슈팅)
6. [git 워크플로](#6-git-워크플로)

---

## 1. 설치

### 1.1 사전 준비물

| 패키지 | 최소 버전 | 비고 |
|---|---|---|
| CMake | 3.22+ | `brew install cmake` |
| ROOT | 6.25+ | **arm64 빌드여야 함** |
| Geant4 | 11.0+ | **Qt 지원 필수** (`find_package(Geant4 REQUIRED COMPONENTS qt)`) |
| Python 3 | 3.x | 개발 헤더 포함 |
| expat | Geant4가 링크한 것과 동일 버전 | 아래 1.3 참고 |

Geant4는 Qt 지원이 켜져 있어야 한다. 확인:

```bash
geant4-config --has-feature qt     # yes 가 나와야 함
geant4-config --version
```

물리 데이터셋도 설치돼 있어야 런타임에 동작한다. 확인:

```bash
geant4-config --datasets
```

`G4NDL`, `G4EMLOW`, `PhotonEvaporation`, `RadioactiveDecay`, `G4PARTICLEXS`, `G4ENSDFSTATE` 등이 경로와 함께 나열되면 정상이다.

### 1.2 아키텍처 함정 — 가장 먼저 확인할 것

**Apple Silicon Mac에서 가장 흔하게 겪는 문제다.** Intel용 Homebrew(`/usr/local`)와 Apple Silicon용 Homebrew(`/opt/homebrew`)가 한 시스템에 공존할 수 있고, ROOT와 Geant4가 서로 다른 아키텍처로 설치돼 있으면 **링크 단계에서 반드시 실패한다.** 아키텍처가 다른 Mach-O 라이브러리는 한 바이너리로 링크할 수 없다.

빌드를 시작하기 전에 두 라이브러리의 아키텍처를 직접 확인하라.

```bash
file $(root-config --libdir)/libCore.so
file $(geant4-config --prefix)/lib/libG4global.dylib
```

둘 다 `arm64`로 나와야 한다. 하나라도 `x86_64`면 그쪽을 다시 설치해야 한다.

`/usr/local/Cellar/root`에 있는 ROOT는 Intel용(x86_64)이다. arm64 ROOT는 다음으로 설치한다.

```bash
/opt/homebrew/bin/brew install root
```

두 ROOT가 공존하면 `PATH` 순서에 따라 어느 쪽이 잡힐지 달라진다. `~/.zshrc`에서 어느 `thisroot.sh`를 source하는지 반드시 확인하라.

```bash
# arm64 ROOT를 쓰도록
source /opt/homebrew/bin/thisroot.sh
```

x86_64 ROOT를 source한 상태로 arm64 바이너리를 실행하면 런타임에 이렇게 죽는다.

```
Fatal in <TROOT::InitInterpreter>: cannot load library .../libRIO.so
  (mach-o file, but is an incompatible architecture (have 'x86_64', need 'arm64'))
```

> **참고 — `CMakeLists.txt`의 하드코딩**
>
> 상위 저장소의 `CMakeLists.txt`에는 `set(CMAKE_OSX_ARCHITECTURES "x86_64")`가 하드코딩돼 있다. `set()`은 캐시 변수를 가리는 일반 변수를 만들기 때문에 **`-DCMAKE_OSX_ARCHITECTURES=arm64`로는 덮어쓸 수 없고**, 파일을 직접 고쳐야 한다.
>
> ```cmake
> set(CMAKE_OSX_ARCHITECTURES "arm64")
> ```
>
> 이 수정은 머신에 종속적이므로 **커밋하지 않고 작업 트리에만 둔다.** `git status`에 항상 `M CMakeLists.txt`로 뜨는 것이 정상이다.

### 1.3 빌드

```bash
cd ratpac-two

cmake . -Bbuild \
  -DROOT_DIR=/opt/homebrew/Cellar/root/6.40.02_1/share/root/cmake \
  -DGeant4_DIR=$HOME/geant4-v11.4.2/geant4-v11.4.2-install/lib/cmake/Geant4 \
  -DEXPAT_INCLUDE_DIR=/opt/homebrew/opt/expat/include \
  -DEXPAT_LIBRARY=/opt/homebrew/opt/expat/lib/libexpat.dylib \
  -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON \
  -DCMAKE_INSTALL_PREFIX=$PWD/install

cmake --build build -j$(sysctl -n hw.ncpu)
cmake --install build
```

경로(ROOT 버전 디렉터리, Geant4 설치 위치)는 각자 환경에 맞게 바꾼다. `-D` 옵션 네 개는 각각 이유가 있다.

**`ROOT_DIR` / `Geant4_DIR`**
`PATH`에 여러 버전이 섞여 있을 때 cmake가 엉뚱한 것을 잡는 것을 막는다. `ROOTConfig.cmake` 위치는 이렇게 찾는다.

```bash
find $(root-config --prefix) -name ROOTConfig.cmake
```

**`EXPAT_INCLUDE_DIR` / `EXPAT_LIBRARY`**
`Geant4Config.cmake`는 Geant4가 빌드될 때 링크했던 expat과 **같은 버전 이상**을 요구한다. MacPorts(`/opt/local`)가 설치돼 있으면 cmake가 그쪽의 구버전을 먼저 찾아서 이렇게 실패한다.

```
CMake Error ... Could NOT find EXPAT: Found unsuitable version "2.5.0",
but required is at least "2.8.3" (found /opt/local/lib/libexpat.dylib)
```

Homebrew expat을 명시적으로 지정하면 해결된다. 요구 버전은 에러 메시지에 그대로 나오므로, 시스템의 expat 버전을 확인해 맞는 것을 고른다.

```bash
grep -E "XML_(MAJOR|MINOR|MICRO)_VERSION" /opt/homebrew/opt/expat/include/expat.h
```

**`CMAKE_INSTALL_RPATH_USE_LINK_PATH=ON`**
**빠뜨리면 설치본이 실행되지 않는다.** cmake는 install 시 빌드 트리의 rpath를 제거하는데, 이 프로젝트는 `INSTALL_RPATH`를 따로 지정하지 않는다. 그래서 이 옵션 없이 설치하면 `install/bin/rat`에 rpath가 하나도 남지 않는다.

```
dyld[...]: Library not loaded: @rpath/libG4Tree.dylib
```

> **동봉된 `Makefile`을 쓰지 말 것**
>
> 저장소 루트의 `Makefile`(`make` 한 방으로 빌드)은 위 옵션들을 넘기지 않는다. 그대로 쓰면 expat을 잘못 찾거나 rpath가 빠진 설치본이 만들어진다. 위의 `cmake` 명령을 직접 쓰는 편이 안전하다.

빌드 산출물은 다음과 같다.

```
install/bin/    rat, ntuple, ratdb2json, root, ratpac.sh, ratpac.csh
install/lib/    libRATPAC.dylib, libRATEvent.dylib, libRATDict_rdict.pcm ...
install/share/RAT/   ratdb, models, macros, python, doc
```

### 1.4 환경 설정

```bash
source install/bin/ratpac.sh
```

이 스크립트가 설정하는 것:

| 변수 | 용도 |
|---|---|
| `RATROOT` | 설치 루트 |
| `RATSHARE` | ratdb / models / macros 경로 |
| `PATH` | `rat` 등 실행 파일 |
| `DYLD_LIBRARY_PATH` | RAT 공유 라이브러리 (macOS는 `LD_LIBRARY_PATH`가 아니라 이것) |
| `PYTHONPATH` | RAT python 모듈 |
| `ROOT_INCLUDE_PATH` | RAT 헤더 (ROOT 매크로에서 `#include` 할 때) |

`ratpac.sh`는 ROOT와 Geant4 환경까지 잡아주지는 **않는다.** 그 둘은 `~/.zshrc`에서 이미 잡혀 있어야 한다.

```bash
# ~/.zshrc
source /opt/homebrew/bin/thisroot.sh
source $HOME/geant4-v11.4.2/geant4-v11.4.2-install/bin/geant4.sh
```

자주 쓴다면 `ratpac.sh`도 `.zshrc`에 추가한다. 단, 여러 빌드를 오가며 쓴다면 그때그때 수동으로 source하는 편이 헷갈리지 않는다.

### 1.5 설치 확인

```bash
rat --version          # RAT, version 2.1.3
rat --help
```

실제 시뮬레이션까지 돌려봐야 제대로 확인된다.

```bash
rat -o test.root $RATSHARE/macros/validation/electron.mac
```

정상이면 마지막에 이런 줄이 나온다.

```
CountProc: Total # of events 100 (91 triggered events)
```

트리거된 이벤트 수는 난수 시드에 따라 매번 조금씩 달라진다(90~95 근처). 출력 파일을 열어본다.

```bash
root -l test.root
root [1] T->GetEntries()          // 100
root [2] runT->Print()
```

---

## 2. 기본 사용법

### 2.1 rat 실행

**배치 모드** — 매크로 파일을 인자로 주면 순서대로 실행하고 종료한다.

```bash
rat macro1.mac macro2.mac
```

**대화형 모드** — 인자 없이 실행하면 대화형 셸이 뜬다. 명령 히스토리와 tcsh 스타일 키 조작을 지원하고 `exit`로 빠져나온다. 현재 디렉터리에 `prerun.mac`이 있으면 셸이 뜨기 전에 먼저 실행된다.

```bash
rat
```

**명령행 옵션** (`rat --help`)

| 옵션 | 설명 |
|---|---|
| `-o, --output` | 기본 출력 파일명 |
| `-i, --input` | 기본 입력 파일명 |
| `-l, --log` | 로그 파일명 |
| `-s, --seed` | 난수 시드 |
| `-r, --run` | 시뮬레이션 run 번호 |
| `-b, --database` | 데이터베이스 URL |
| `-p, --python` | python 프로세서 |
| `-x, --vector` | 기본 vector 파일명 |
| `-g, --vis` | G4UI 시각화 로드 |
| `-d, --debug` / `-v, --verbose` / `-q, --quiet` | 로그 상세도 |
| `-V, --version` | 버전 출력 후 종료 |

`-o`로 준 출력 파일명은 매크로 안의 `/rat/procset file`보다 우선한다. 같은 매크로로 여러 조건을 돌릴 때 유용하다.

### 2.2 매크로 파일 구조

Ratpac은 Geant4의 명령 인터페이스를 그대로 쓴다. `#`으로 주석을 달고, 빈 줄은 무시된다. **명령 앞에 공백을 넣을 수 없다** — 들여쓰기로 가독성을 높이는 것이 불가능하다.

명령의 순서가 중요하므로 아래 구조를 지키는 것이 좋다.

```
# 1. 파라미터 설정 (/run/initialize 이전이어야 함)
/rat/db/set DETECTOR experiment "Validation"
/rat/db/set DETECTOR geo_file "Validation/Valid.geo"

# 2. 초기화 — 지오메트리 확정
/run/initialize

# 3. 프로세서 등록 (이벤트 루프 구성)
/rat/proc splitevdaq
/rat/proc count
/rat/procset update 100
/rat/proclast outroot

# 4. 생성기 설정
/generator/add combo gun:point:poisson
/generator/vtx/set e- 0.0 0.0 0.0 2.5
/generator/pos/set 0.0 0.0 0.0

# 5. 실행
/run/beamOn 100
```

**핵심은 `/run/initialize`를 경계로 앞뒤가 나뉜다는 점이다.** 지오메트리와 데이터베이스 설정(`/rat/db/set`)은 그 앞에, 프로세서와 생성기 설정은 그 뒤에 와야 한다.

다른 매크로를 포함시킬 수 있다. 해당 위치에서 즉시 읽어 실행된다.

```
/control/execute setup.mac
```

### 2.3 검출기 선택

`ratdb/` 아래에 예제 검출기 정의가 들어 있다.

| 실험 | geo 파일 | 용도 |
|---|---|---|
| `Validation` | `Validation/Valid.geo` | 검증용 소형 검출기. 가장 빠르다 |
| `SNO` | `SNO/sno.geo` | SNO 검출기 |
| `SuperK` | `SuperK/SuperK.geo` | Super-Kamiokande |

```
/rat/db/set DETECTOR experiment "Validation"
/rat/db/set DETECTOR geo_file "Validation/Valid.geo"
```

`ratdb/*.ratdb` 파일들은 물질(`MATERIALS`), 광학 상수(`OPTICS`), PMT 특성(`PMT`, `PMTCHARGE`, `PMTTRANSIT`), DAQ 설정(`DAQ`, `DIGITIZER`), 물리 리스트(`PHYSICS`) 등을 담는다. 매크로에서 개별 항목을 덮어쓸 수 있다.

```
/rat/db/set DAQ trigger_threshold 10.0
```

### 2.4 이벤트 생성기

생성기는 **언제, 어디서, 어떤 입자가** 발생하는지를 결정한다. 여러 개를 동시에 등록할 수 있고, 각자의 발생률에 따라 이벤트가 섞이거나 동시에 쌓인다.

```
/generator/add <생성기이름> <옵션문자열>
```

가장 많이 쓰는 것은 `combo`로, **vertex : position : time** 세 요소를 조합한다.

```
/generator/add combo gun:point:poisson
```

TIME을 생략하면 `poisson`이 기본값이다.

**최상위 생성기**

| 이름 | 설명 |
|---|---|
| `combo` | vertex/position/time 조합 |
| `decaychain` | 붕괴 사슬 |
| `cfsource` | Cf 선원 |
| `led` | LED 광원 |
| `external` | 외부 입력 |
| `vertexfile` | 파일에서 vertex 읽기 |

**vertex 생성기** — `/generator/vtx/set`으로 설정

| 이름 | 설명 |
|---|---|
| `gun` | 단일 입자총 |
| `gun2` | 에너지 범위를 갖는 입자총 |
| `ibd` | 역베타붕괴 (양전자 + 중성자) |
| `reacibd` | 원자로 중성미자 IBD |
| `es` | 탄성 산란 |
| `spectrum` | 스펙트럼 기반 |
| `pbomb` | 광자 폭탄 (광학 테스트용) |

`gun`의 문법:

```
/generator/vtx/set  입자명  px  py  pz  [운동에너지]  [polx poly polz]
```

- 운동량 `(px, py, pz)`는 MeV/c
- 운동에너지(MeV)를 주면 운동량 벡터의 크기를 덮어쓴다 → **이때 `px,py,pz`는 방향 벡터로만 쓰인다**
- `px=py=pz=0`이면 등방(isotropic) 발생
- 편광 벡터를 생략하거나 0으로 주면 무작위 편광

```
# 2.5 MeV 전자를 등방으로
/generator/vtx/set e- 0.0 0.0 0.0 2.5
```

입자명은 Geant4 표준 이름이다: `e-`, `e+`, `gamma`, `neutron`, `proton`, `mu-`, `mu+`, `alpha`, `opticalphoton`, `GenericIon` 등. 전체 목록은 `/particle/list`로 볼 수 있다.

`ibd`의 문법 — 중성미자 방향만 준다. 에너지는 `IBD` 테이블의 스펙트럼에서 뽑고, 양전자 방향은 미분단면적으로 가중된다.

```
/generator/vtx/set  dir_x dir_y dir_z
```

**position 생성기** — `/generator/pos/set`으로 설정

| 이름 | 설명 |
|---|---|
| `point` | 한 점 `(x, y, z)`, 단위 mm |
| `fill` | 점 `(x,y,z)`를 포함하는 물리 볼륨을 균일하게 채움 |
| `fillshell` | 껍질 영역을 채움 |
| `multipoint` | 여러 점 |
| `paint` | 표면에 도포 |
| `regexfill` | 정규식으로 고른 볼륨을 채움 |

**time 생성기** — `/generator/rate/set`으로 설정

| 이름 | 설명 |
|---|---|
| `poisson` | 평균 `evrate`(1/초)의 포아송 분포 |
| `uniform` | 균일 간격 |

```
/generator/rate/set 1.0     # 초당 평균 1회
```

### 2.5 프로세서

프로세서는 이벤트 하나를 받아 처리하는 모듈이다. 매크로에 나열한 순서대로 이벤트 루프가 구성된다.

```
/rat/proc <이름>              # 스택에 추가
/rat/proclast <이름>          # 스택 맨 끝에 추가 (명령행 프로세서보다 뒤)
/rat/procset <파라미터> <값>   # 직전에 추가한 프로세서의 파라미터 설정
```

`/rat/procset`은 반드시 `/rat/proc` **다음에** 와야 한다. 그렇지 않으면 실행이 중단된다.

**사용 가능한 프로세서** (`src/cmd/src/ProcBlockManager.cc` 등록 순)

| 분류 | 이름 | 설명 |
|---|---|---|
| 출력 | `outroot` | 전체 데이터 구조를 ROOT로 저장 |
| | `outntuple` | 평평한 ntuple로 저장 |
| | `outnet` | 네트워크로 전송 |
| DAQ | `simpledaq` | 최소 DAQ. 모든 PMT 히트를 이벤트 하나로 묶음 |
| | `splitevdaq` | 시간으로 분리해 여러 검출기 이벤트 생성 |
| | `lesssimpledaq`, `lesssimpledaq2`, `truedaq` | 그 외 DAQ 모델 |
| | `noise` | 암전류 노이즈 추가 |
| | `afterpulse` | PMT 애프터펄스 |
| 재구성 | `fitcentroid` | 전하 가중 중심으로 위치 재구성 |
| | `fitpath` | PathFitter — 위치·시간·방향 (FTP 테이블 사용) |
| | `fittensor` | TensorFlow 기반 (빌드 시 TF 필요, 이 환경에는 없음) |
| 분류 | `classifychargebalance` | 전하 균형 분류자 |
| 기타 | `count` | 진행 상황 출력 |
| | `prune` | 데이터 구조 일부 제거 |
| | `python` | python 프로세서 |

**`count`** — 진행 상황을 주기적으로 출력한다.

```
/rat/proc count
/rat/procset update 100      # 100 이벤트마다 (기본값 1)
```

```
CountProc: Event 100 (94 triggered events)
```

**`prune`** — 디스크 용량을 줄이려면 `outroot` **앞에** 놓는다. 쉼표로 구분하고 **공백을 넣지 않는다.**

```
/rat/proc prune
/rat/procset prune "mc.track,mc.pmt.photon"
```

제거 가능한 항목: `mc.particle`, `mc.pmt`, `mc.pmt.photon`, `mc.track`, `ev`, `ev.pmt`

`/tracking/storeTrajectory`가 켜져 있으면 입자별로도 지정할 수 있다 (`mc.track:opticalphoton`).

메모리 절약 효과는 거의 없다. 메모리에는 항상 데이터 구조 하나만 존재하기 때문이다. **디스크 용량을 줄이는 용도로만 의미가 있다.**

### 2.6 출력

**`outroot`** — 전체 데이터 구조를 저장한다. 정보 손실이 없지만 파일이 크다.

```
/rat/proclast outroot
/rat/procset file "output.root"
```

파일이 이미 존재하면 **삭제 후 새로 쓴다.** 명령행 `-o`가 이 설정보다 우선한다.

생성되는 것:

| 이름 | 내용 |
|---|---|
| `T` (TTree) | 이벤트별 데이터. 브랜치 `ds` (`RAT::DS::Root`) |
| `runT` (TTree) | run 정보 |
| `macro` (TObjString) | 실행에 쓰인 매크로 전문 |
| `log` (TObjString) | 로그 |
| `db` (TMap) | 사용된 데이터베이스 값 |

매크로와 DB 값이 파일에 함께 저장되므로, **결과 파일만 있으면 어떤 설정으로 돌렸는지 나중에 재구성할 수 있다.**

**`outntuple`** — 분석하기 쉬운 평평한 TTree를 만든다. 파일이 훨씬 작다 (위 검증 예제 기준 18.7 MB → 60 KB).

```
/rat/proclast outntuple
```

`output` TTree에 38개 브랜치가 생긴다.

| 분류 | 브랜치 |
|---|---|
| MC 진리값 | `mcpdg` `mcx` `mcy` `mcz` `mcu` `mcv` `mcw` `mcke` `mct` |
| 이벤트 | `evid` `subev` `nhits` `triggerTime` |
| 카운트 | `mcparticlecount` `mcpecount` `mcnhits` |
| 에너지 침적 | `scintEdep` `scintEdepQuenched` `scintPhotons` `remPhotons` `cherPhotons` |
| PMT 히트 | `hitPMTID` `hitPMTTime` `hitPMTCharge` `hitPMTDigitizedTime` `hitPMTDigitizedCharge` `hitPMTNCrossings` |
| MC PE | `mcPMTID` `mcPMTNPE` `mcPMTCharge` `mcPEHitTime` `mcPEFrontEndTime` `mcPEProcess` `mcPEWavelength` `mcPEx` `mcPEy` `mcPEz` `mcPECharge` |

`mcx~mcw`는 **첫 번째 입자의** 위치·방향이고, `mcke`는 모든 MC 입자의 운동에너지 **합**이다.

전체 예제:

```
/rat/db/set DETECTOR experiment "Validation"
/rat/db/set DETECTOR geo_file "Validation/Valid.geo"
/run/initialize
/rat/proc splitevdaq
/rat/proc count
/rat/procset update 10
/rat/proclast outntuple
/generator/add combo gun:point:poisson
/generator/vtx/set e- 0.0 0.0 0.0 2.5
/generator/pos/set 0.0 0.0 0.0
/run/beamOn 20
```

---

## 3. 결과 분석

`outntuple` 출력은 평평한 TTree라 바로 그릴 수 있다.

```cpp
root -l output.root
root [1] output->Draw("nhits")
root [2] output->Draw("mcke", "nhits > 10")
root [3] output->Draw("mcy:mcx", "", "colz")
```

`outroot` 출력은 `RAT::DS::Root` 객체를 다루므로 RAT 라이브러리가 필요하다. `ratpac.sh`를 source한 뒤 `root`를 실행하면 `RAT: Libraries loaded.`가 뜨면서 라이브러리가 자동으로 로드된다.

```cpp
root -l output.root
root [1] T->GetEntries()
```

C++ 매크로로 순회하는 예:

```cpp
void analyze() {
  TFile f("output.root");
  TTree *T = (TTree*)f.Get("T");

  RAT::DS::Root *ds = new RAT::DS::Root();
  T->SetBranchAddress("ds", &ds);

  TH1F *h = new TH1F("h", ";PMT hits;events", 100, 0, 200);

  for (Long64_t i = 0; i < T->GetEntries(); i++) {
    T->GetEntry(i);
    for (int iev = 0; iev < ds->GetEVCount(); iev++) {
      h->Fill(ds->GetEV(iev)->GetPMTCount());
    }
  }
  h->Draw();
}
```

데이터 구조는 `RAT::DS` 네임스페이스에 정의돼 있다 (`src/ds/`). `RAT::DS::Root` 하나가 물리 이벤트 하나에 대응하며, 그 안에 MC 진리값(`RAT::DS::MC`)과 검출기 이벤트 목록(`RAT::DS::EV`)이 들어 있다. **물리 이벤트 하나가 검출기 이벤트 여러 개로 쪼개질 수 있다** — 예를 들어 IBD는 양전자와 중성자 포획 사이에 시간 지연이 있어 최소 두 개의 검출기 이벤트로 관측된다.

---

## 4. 이 포크의 로컬 수정 사항

상위 저장소는 Geant4 11.4 / ROOT 6.40에서 빌드되지 않는다. 이 포크에는 네 건의 수정이 커밋돼 있다 (커밋 `7cd69f4`).

| 파일 | 내용 |
|---|---|
| `src/geo/src/gdml/GDMLWriteStructure.cc` | Geant4 11.3부터 `G4LogicalSkinSurfaceTable`이 vector에서 `std::map<const G4LogicalVolume*, G4LogicalSkinSurface*>`로 바뀌었다. `pos->first` / `pos->second`로 접근하도록 수정 |
| `src/core/src/GLG4VisManager.cc` | Geant4 11.4에서 `G4HepRepFile` 시각화 드라이버가 삭제됐다. include와 등록 제거 |
| `src/gen/src/Decay0.cc` | ROOT 6.36에서 인터프리터용 `TF1(name, void*, ..., className)` 생성자가 제거됐다. 멤버 함수 포인터 생성자로 교체 |
| `src/core/src/PythonProc.cc` | ROOT 6.36에서 `TPython::Eval`이 제거됐다. `TPython::Exec(cmd, &std::any)` 패턴으로 교체 |

> **하위 호환성 주의**
>
> 이 수정들은 **Geant4 11.3 미만 / ROOT 6.36 미만에서는 오히려 빌드를 깨뜨린다.** 구버전 환경과 코드를 공유한다면 그대로 가져가지 말 것.

여기에 더해, 커밋하지 않고 작업 트리에만 두는 수정이 하나 있다.

| 파일 | 내용 |
|---|---|
| `CMakeLists.txt` | `CMAKE_OSX_ARCHITECTURES`를 `x86_64` → `arm64` |

머신에 종속적이라 커밋하지 않는다. 커밋할 때는 파일을 명시적으로 지정해 실수로 섞이지 않게 한다.

```bash
git add src/... src/...      # git add -A 를 쓰지 말 것
```

---

## 5. 트러블슈팅

### 빌드 시 expat 버전 오류

```
Could NOT find EXPAT: Found unsuitable version "2.5.0",
but required is at least "2.8.3" (found /opt/local/lib/libexpat.dylib)
```

MacPorts의 구버전 expat을 먼저 찾은 것이다. Homebrew expat을 명시한다 → [1.3](#13-빌드)

### 실행 시 `Library not loaded: @rpath/libG4Tree.dylib`

설치본에 rpath가 없다. `-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON`을 넣고 다시 빌드·설치한다. 확인:

```bash
otool -l install/bin/rat | grep -A2 LC_RPATH | grep path
```

ROOT, Geant4, 프로젝트 lib 경로가 나와야 한다.

### 실행 시 `incompatible architecture (have 'x86_64', need 'arm64')`

x86_64 ROOT가 잡혀 있다. 어느 ROOT가 활성인지 확인한다.

```bash
which root-config
file $(root-config --libdir)/libCore.so
```

`~/.zshrc`에서 `/opt/homebrew/bin/thisroot.sh`를 source하도록 고친다 → [1.2](#12-아키텍처-함정--가장-먼저-확인할-것)

### `-Wno-terminate` 경고

```
warning: unknown warning option '-Wno-terminate' [-Wunknown-warning-option]
```

`CMakeLists.txt`가 GCC용 옵션을 무조건 넣는데 clang에는 없는 옵션이다. **무해하며 빌드에 영향이 없다.** 파일마다 한 줄씩 나오므로 로그가 길어질 뿐이다.

### `sprintf is deprecated` 경고

macOS SDK가 붙이는 경고다. 상위 코드의 문제이며 동작에는 영향이 없다.

### `ntuple` 명령이 segmentation fault로 죽음

```
 *** Break *** segmentation violation
[libRATPAC.dylib] RAT::OutNtupleProc::DSEvent(RAT::DS::Root*)
```

**독립 실행 도구 `ntuple`의 알려진 버그다.** 이 포크의 수정과는 무관하며 상위 저장소에도 그대로 있다.

원인: `tools/ntuple/ntuple.cpp`가 `runT` 트리에서 `proc.runBranch`를 직접 설정하지만, `OutNtupleProc::DSEvent()`가 첫 줄에서 `runBranch = DS::RunStore::GetRun(ds)`로 이를 덮어쓴다. 독립 도구는 `DS::RunStore::SetReadTree()`를 호출하지 않으므로 `GetRun()`이 `nullptr`을 반환하고, 다음 줄의 `runBranch->GetPMTInfo()`에서 죽는다. (`InROOTProducer`는 `SetReadTree()`를 호출하기 때문에 `rat` 안에서는 문제가 없다.)

**우회 방법: 매크로에서 `outntuple` 프로세서를 쓴다.** 결과물은 동일하다.

```
/rat/proclast outntuple
```

### 시각화가 열리지 않음

Geant4가 Qt 지원으로 빌드돼 있어야 한다.

```bash
geant4-config --has-feature qt      # yes
```

`no`가 나오면 Geant4를 `-DGEANT4_USE_QT=ON`으로 다시 빌드해야 한다. Ratpac은 `find_package(Geant4 REQUIRED COMPONENTS qt)`로 Qt를 필수 요구하므로, Qt 없는 Geant4로는 애초에 cmake 설정 단계에서 실패한다.

예제 매크로는 `/vis/open OGLSQt`로 뷰어를 직접 열기 때문에 그냥 실행하면 된다.

```bash
rat $RATSHARE/macros/validation/visualize.mac
```

뷰어를 열지 않는 매크로에 시각화를 붙이려면 `-g` 옵션을 쓴다.

---

## 6. git 워크플로

```
origin    github.com/Sang-Yong/ratpac-two_for_RENE_on_mac   작업용
upstream  github.com/cpnr/ratpac-two                        읽기 전용
```

작업 브랜치는 `RENE`다. 상위 변경을 받아오려면:

```bash
git fetch upstream
git merge upstream/RENE
```

`CMakeLists.txt`가 항상 수정 상태이므로 merge 시 충돌할 수 있다. 그때는 `arm64` 쪽을 유지한다.

---

## 참고 자료

- 상위 문서: [ratpac.readthedocs.io](https://ratpac.readthedocs.io)
- 원본 저장소: [github.com/rat-pac/ratpac-two](https://github.com/rat-pac/ratpac-two)
- 의존성 설치 도우미: [github.com/rat-pac/ratpac-setup](https://github.com/rat-pac/ratpac-setup)
- 실험 패키지 예제: [github.com/rat-pac/RatpacExperiment](https://github.com/rat-pac/RatpacExperiment)
- 저장소 내 영문 문서: `doc/users_guide/`
