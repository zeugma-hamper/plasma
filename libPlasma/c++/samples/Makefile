all:
	set -ex; for dir in *; do if test -f $$dir/CMakeLists.txt; then cd $$dir; obi build; cd ..; fi; done

clean:
	set -ex; for dir in *; do if test -f $$dir/CMakeLists.txt; then cd $$dir; obi clean; cd ..; fi; done

bauall:
	set -ex; for dir in *; do if test -f $$dir/CMakeLists.txt; then cd $$dir; bau build $(BAUOPTS); cd ..; fi; done

bauclean:
	set -ex; for dir in *; do if test -f $$dir/CMakeLists.txt; then cd $$dir; bau clean; cd ..; fi; done
