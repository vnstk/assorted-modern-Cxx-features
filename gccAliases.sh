#/*   (c)2024 Vainstein K.   */
# NB: before aliasing, we unalias; that way, OK to re-source this file after changing it.

# A few other oft-utile switches:
#
#	-E		Halt after preprocessing but before compiling.
#	-S		Halt after compiling but before assembling.
#	-c		Halt after assembling but before linking.
#
#	-v		Print actual commands to individual components (cpp, gcc, gas, ldd, ...)

commonFlags_debug=''                             #### For the best poss debug experience.
commonFlags_debug+=' -fdebug-types-section'
#commonFlags_debug+=' -femit-struct-debug-detailed=all'
commonFlags_debug+=' -fno-eliminate-unused-debug-types'
commonFlags_debug+=' -fno-merge-debug-strings'
commonFlags_debug+=' -fvar-tracking'
commonFlags_debug+=' -fvar-tracking-assignments'
commonFlags_debug+=' -gdwarf-5'
commonFlags_debug+=' -ggdb3'
commonFlags_debug+=' -ginline-points'
commonFlags_debug+=' -ginternal-reset-location-views'
commonFlags_debug+=' -gstatement-frontiers'
commonFlags_debug+=' -gvariable-location-views '
commonFlags_debug+=' -O0'


cppFlags_base=''
cppFlags_base+=' -fcoroutines'
cppFlags_base+=' -fdiagnostics-path-format=inline-events'
cppFlags_base+=' -fdiagnostics-show-template-tree'	# Pretty-print mismatching te types
cppFlags_base+=' -fdiagnostics-text-art-charset=unicode'
cppFlags_base+=' -fno-exceptions'
cppFlags_base+=' -fno-diagnostics-show-cwe'			# Don't need.
cppFlags_base+=' -fno-modules'
cppFlags_base+=' -fno-operator-names'				# Gaaah!!
cppFlags_base+=' -grecord-gcc-switches'
													#### Static runs faster, debugs easier.
cppFlags_base+=' -static'
cppFlags_base+=' -static-libgcc'
cppFlags_base+=' -static-libstdc++'
													#### printf(3) and scanf(3) warnings
cppFlags_base+=' -Wformat=1'
cppFlags_base+=' -Wformat-overflow=1'
cppFlags_base+=' -Wformat-truncation=1'
#cppFlags_base+=' -Wno-format-signedness' # Ignored?
													#### Officious warnings.
cppFlags_base+=' -Wno-class-conversion'
cppFlags_base+=' -Wno-endif-labels'
cppFlags_base+=' -Wno-misleading-indentation'
cppFlags_base+=' -Wno-system-headers'
cppFlags_base+=' -Wno-old-style-cast'
cppFlags_base+=' -Wno-parentheses'
cppFlags_base+=' -Wno-xor-used-as-pow'
													#### Salutary warnings.
cppFlags_base+=' -Warray-bounds=1'
cppFlags_base+=' -Wclobbered'			# vars that might be changed by "longjmp" or "vfork"
cppFlags_base+=' -Wdangling-else'
cppFlags_base+=' -Wdangling-reference'
cppFlags_base+=' -Wdeprecated-copy'
cppFlags_base+=' -Wduplicated-branches'
cppFlags_base+=' -Wduplicated-cond'
cppFlags_base+=' -Wfree-nonheap-object'
cppFlags_base+=' -Winfinite-recursion'
cppFlags_base+=' -Winit-self'
cppFlags_base+=' -Wmismatched-dealloc'
cppFlags_base+=' -Wmismatched-new-delete'
cppFlags_base+=' -Wmissing-include-dirs'
cppFlags_base+=' -Wmissing-template-keyword'
cppFlags_base+=' -Wmultistatement-macros'
#cppFlags_base+=' -Wnoexcept' #Warn when a noexcept-expression evals to false because of a call to a function which lacks a non-throwing exception specification (i.e. throw() or noexcept) but is known by the compiler to never throw an exception.
cppFlags_base+=' -Wnoexcept-type'
cppFlags_base+=' -Wnrvo' # Warn when move semantics don't translate to move actuality: most cool!!!!
cppFlags_base+=' -Wodr'
cppFlags_base+=' -Woverloaded-virtual' # Warn when a fu decl hides virtual fu from a base class. 
cppFlags_base+=' -Wpacked-not-aligned'
cppFlags_base+=' -Wpessimizing-move' # warns when call to std::move prevents copy elision
cppFlags_base+=' -Wrange-loop-construct' # warns when a range-based for-loop is creating an unnecessary copy.
cppFlags_base+=' -Wredundant-move' # warns when move operation would have been performed even without the std::move call
cppFlags_base+=' -Wreorder' # Warn when order of memb initializers given in the code does not match the order in which they must be executed
cppFlags_base+=' -Wrestrict'
cppFlags_base+=' -Wself-move'
cppFlags_base+=' -Wsequence-point'
cppFlags_base+=' -Wshadow=local'
cppFlags_base+=' -Wsizeof-array-div'
cppFlags_base+=' -Wsizeof-pointer-div'
cppFlags_base+=' -Wstring-compare'
cppFlags_base+=' -Wstringop-overflow=2'
cppFlags_base+=' -Wtautological-compare'
cppFlags_base+=' -Wtrigraphs'
cppFlags_base+=' -Wtype-limits'
#cppFlags_base+=' -Wuninitialized'   #Triggered by sys headers, even though -Wno-system-headers; weird.
cppFlags_base+=' -Wuse-after-free=2'

cppFlags_debug="${commonFlags_debug}"
cppFlags_debug+=' -fdump-tree-gimple=Expanded_Source.cxx'
cppFlags_debug+=' -femit-class-debug-always'

cppCmdline_base="g++ ${cppFlags_base} "'-Og -std=c++'

unalias p11 p14 p17 p20 p23 p26 2>/dev/null
alias p11="${cppCmdline_base}11 "
alias p14="${cppCmdline_base}14 "
alias p17="${cppCmdline_base}17 "
alias p20="${cppCmdline_base}20 "
alias p23="${cppCmdline_base}23 "
alias p26="${cppCmdline_base}26 "

unalias dbg_p11 dbg_p14 dbg_p17 dbg_p20 dbg_p23 dbg_p26 2>/dev/null
alias dbg_p11="${cppCmdline_base}11 ${cppFlags_debug} "
alias dbg_p14="${cppCmdline_base}14 ${cppFlags_debug} "
alias dbg_p17="${cppCmdline_base}17 ${cppFlags_debug} "
alias dbg_p20="${cppCmdline_base}20 ${cppFlags_debug} "
alias dbg_p23="${cppCmdline_base}23 ${cppFlags_debug} "
alias dbg_p26="${cppCmdline_base}26 ${cppFlags_debug} "

unset cppFlags_base cppFlags_debug cCmdline_base


# ref  https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html


cFlags_base=''
cFlags_base+=' -grecord-gcc-switches'
cFlags_base+=' -static'
cFlags_base+=' -static-libgcc'
cFlags_base+=' -Warray-bounds=1'
cFlags_base+=' -Wbad-function-cast'  # Warn if cast func to an incompatible func.
cFlags_base+=' -Wcast-function-type' # Warn if cast func to a non-func.
cFlags_base+=' -Wduplicated-branches'
cFlags_base+=' -Wduplicated-cond'
cFlags_base+=' -Wenum-conversion'
cFlags_base+=' -Wformat=1'
cFlags_base+=' -Wformat-overflow=1'
cFlags_base+=' -Wformat-truncation=1'
cFlags_base+=' -Wfree-nonheap-object'
cFlags_base+=' -Wimplicit-function-declaration'
cFlags_base+=' -Winit-self'
cFlags_base+=' -Winline' # Warn if compiler cannot oblige an "inline" request.
cFlags_base+=' -Wint-conversion'
cFlags_base+=' -Wjump-misses-init'
cFlags_base+=' -Wmismatched-dealloc'
cFlags_base+=' -Wmissing-include-dirs'
cFlags_base+=' -Wmissing-parameter-type'
cFlags_base+=' -Wmultistatement-macros'
cFlags_base+=' -Wpacked-not-aligned'
cFlags_base+=' -Wrestrict'
cFlags_base+=' -Wreturn-mismatch'
cFlags_base+=' -Wshadow=local'
cFlags_base+=' -Wsizeof-array-div'
cFlags_base+=' -Wsizeof-pointer-div'
cFlags_base+=' -Wstring-compare'
cFlags_base+=' -Wstringop-overflow'
cFlags_base+=' -Wtautological-compare'
cFlags_base+=' -Wtrigraphs'
cFlags_base+=' -Wuninitialized'   #Hmm, hmm.  We'll see.
cFlags_base+=' -Wunused-label'
cFlags_base+=' -Wunused-local-typedefs'
cFlags_base+=' -Wunused-variable'
cFlags_base+=' -Wuse-after-free=2'

cFlags_debug="${commonFlags_debug}"
cFlags_debug+=' -aux-info Aux_Info.c' # Exhaustive listing of all involved declarations.

cCmdline_base="gcc ${cFlags_base} "'-x c -Og -std=c'

unalias ckr c90 c99 c11 c17 c23 2>/dev/null
alias ckr="${cCmdline_base}90 "
alias c90="${cCmdline_base}90 "
alias c99="${cCmdline_base}99 "
alias c11="${cCmdline_base}11 "
alias c17="${cCmdline_base}17 "
alias c23="${cCmdline_base}23 "

unalias dbg_ckr dbg_c90 dbg_c99 dbg_c11 dbg_c17 dbg_c23 2>/dev/null
alias dbg_ckr="${cCmdline_base}90 ${cFlags_debug}"
alias dbg_c90="${cCmdline_base}90 ${cFlags_debug}"
alias dbg_c99="${cCmdline_base}99 ${cFlags_debug}"
alias dbg_c11="${cCmdline_base}11 ${cFlags_debug}"
alias dbg_c17="${cCmdline_base}17 ${cFlags_debug}"
alias dbg_c23="${cCmdline_base}23 ${cFlags_debug}"

unset cFlags_base cFlags_debug cCmdline_base

unset commonFlags_debug

# To view a definition, say e.g.    type p17
