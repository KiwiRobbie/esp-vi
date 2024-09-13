###############################################################################

set(XSRC)                         
list(APPEND XSRC openbsd/dirname.c     )
list(APPEND XSRC openbsd/err.c         )
list(APPEND XSRC openbsd/errc.c        )
list(APPEND XSRC openbsd/errx.c        )
list(APPEND XSRC openbsd/getopt_long.c )
list(APPEND XSRC openbsd/getprogname.c )
list(APPEND XSRC openbsd/issetugid.c   )
list(APPEND XSRC openbsd/minpwcache.c  )
list(APPEND XSRC openbsd/reallocarray.c)
list(APPEND XSRC openbsd/setmode.c     )
list(APPEND XSRC openbsd/strlcat.c     )
list(APPEND XSRC openbsd/strlcpy.c     )
list(APPEND XSRC openbsd/strtonum.c    )
list(APPEND XSRC openbsd/verr.c        )
list(APPEND XSRC openbsd/verrc.c       )
list(APPEND XSRC openbsd/verrx.c       )
list(APPEND XSRC openbsd/vwarn.c       )
list(APPEND XSRC openbsd/vwarnc.c      )
list(APPEND XSRC openbsd/vwarnx.c      )
list(APPEND XSRC openbsd/warn.c        )
list(APPEND XSRC openbsd/warnc.c       )
list(APPEND XSRC openbsd/warnx.c       )
list(APPEND XSRC patch/patch_term.c    )
# list(APPEND XSRC mcurses/editor.c    )
list(APPEND XSRC mcurses/mcurses.c    )


set(SRC)                          
list(APPEND SRC cl/cl_funcs.c           )
list(APPEND SRC cl/cl_main.c            )
list(APPEND SRC cl/cl_read.c            )
list(APPEND SRC cl/cl_screen.c          )
list(APPEND SRC cl/cl_term.c            )
list(APPEND SRC common/cut.c            )
list(APPEND SRC common/delete.c         )
list(APPEND SRC common/exf.c            )
list(APPEND SRC common/key.c            )
list(APPEND SRC common/line.c           )
list(APPEND SRC common/log.c            )
list(APPEND SRC common/main.c           )
list(APPEND SRC common/mark.c           )
list(APPEND SRC common/msg.c            )
list(APPEND SRC common/options.c        )
list(APPEND SRC common/options_f.c      )
list(APPEND SRC common/put.c            )
list(APPEND SRC common/recover.c        )
list(APPEND SRC common/screen.c         )
list(APPEND SRC common/search.c         )
list(APPEND SRC common/seq.c            )
list(APPEND SRC common/util.c           )
list(APPEND SRC db/btree/bt_close.c     )
list(APPEND SRC db/btree/bt_conv.c      )
list(APPEND SRC db/btree/bt_debug.c     )
list(APPEND SRC db/btree/bt_delete.c    )
list(APPEND SRC db/btree/bt_get.c       )
list(APPEND SRC db/btree/bt_open.c      )
list(APPEND SRC db/btree/bt_overflow.c  )
list(APPEND SRC db/btree/bt_page.c      )
list(APPEND SRC db/btree/bt_put.c       )
list(APPEND SRC db/btree/bt_search.c    )
list(APPEND SRC db/btree/bt_seq.c       )
list(APPEND SRC db/btree/bt_split.c     )
list(APPEND SRC db/btree/bt_utils.c     )
list(APPEND SRC db/db/db.c              )
list(APPEND SRC db/hash/hash_bigkey.c   )
list(APPEND SRC db/hash/hash_buf.c      )
list(APPEND SRC db/hash/hash.c          )
list(APPEND SRC db/hash/hash_func.c     )
list(APPEND SRC db/hash/hash_log2.c     )
list(APPEND SRC db/hash/hash_page.c     )
list(APPEND SRC db/hash/ndbm.c          )
list(APPEND SRC db/mpool/mpool.c        )
list(APPEND SRC db/recno/rec_close.c    )
list(APPEND SRC db/recno/rec_delete.c   )
list(APPEND SRC db/recno/rec_get.c      )
list(APPEND SRC db/recno/rec_open.c     )
list(APPEND SRC db/recno/rec_put.c      )
list(APPEND SRC db/recno/rec_search.c   )
list(APPEND SRC db/recno/rec_seq.c      )
list(APPEND SRC db/recno/rec_utils.c    )
list(APPEND SRC ex/ex_abbrev.c          )
list(APPEND SRC ex/ex_append.c          )
list(APPEND SRC ex/ex_args.c            )
list(APPEND SRC ex/ex_argv.c            )
list(APPEND SRC ex/ex_at.c              )
list(APPEND SRC ex/ex_bang.c            )
list(APPEND SRC ex/ex.c                 )
list(APPEND SRC ex/ex_cd.c              )
list(APPEND SRC ex/ex_cmd.c             )
list(APPEND SRC ex/ex_delete.c          )
list(APPEND SRC ex/ex_display.c         )
list(APPEND SRC ex/ex_edit.c            )
list(APPEND SRC ex/ex_equal.c           )
list(APPEND SRC ex/ex_file.c            )
list(APPEND SRC ex/ex_filter.c          )
list(APPEND SRC ex/ex_global.c          )
list(APPEND SRC ex/ex_init.c            )
list(APPEND SRC ex/ex_join.c            )
list(APPEND SRC ex/ex_map.c             )
list(APPEND SRC ex/ex_mark.c            )
list(APPEND SRC ex/ex_mkexrc.c          )
list(APPEND SRC ex/ex_move.c            )
list(APPEND SRC ex/ex_open.c            )
list(APPEND SRC ex/ex_preserve.c        )
list(APPEND SRC ex/ex_print.c           )
list(APPEND SRC ex/ex_put.c             )
list(APPEND SRC ex/ex_quit.c            )
list(APPEND SRC ex/ex_read.c            )
list(APPEND SRC ex/ex_screen.c          )
list(APPEND SRC ex/ex_set.c             )
list(APPEND SRC ex/ex_shell.c           )
list(APPEND SRC ex/ex_shift.c           )
list(APPEND SRC ex/ex_source.c          )
list(APPEND SRC ex/ex_stop.c            )
list(APPEND SRC ex/ex_subst.c           )
list(APPEND SRC ex/ex_tag.c             )
list(APPEND SRC ex/ex_txt.c             )
list(APPEND SRC ex/ex_undo.c            )
list(APPEND SRC ex/ex_usage.c           )
list(APPEND SRC ex/ex_util.c            )
list(APPEND SRC ex/ex_version.c         )
list(APPEND SRC ex/ex_visual.c          )
list(APPEND SRC ex/ex_write.c           )
list(APPEND SRC ex/ex_yank.c            )
list(APPEND SRC ex/ex_z.c               )
list(APPEND SRC openbsd/basename.c      )
list(APPEND SRC openbsd/err.c           )
list(APPEND SRC openbsd/errx.c          )
list(APPEND SRC openbsd/getopt_long.c   )
list(APPEND SRC openbsd/getprogname.c   )
list(APPEND SRC openbsd/issetugid.c     )
list(APPEND SRC openbsd/open.c          )
list(APPEND SRC openbsd/pledge.c        )
list(APPEND SRC openbsd/reallocarray.c  )
list(APPEND SRC openbsd/strlcpy.c       )
list(APPEND SRC openbsd/strtonum.c      )
list(APPEND SRC openbsd/verr.c          )
list(APPEND SRC openbsd/verrx.c         )
list(APPEND SRC openbsd/vwarn.c         )
list(APPEND SRC openbsd/vwarnx.c        )
list(APPEND SRC openbsd/warn.c          )
list(APPEND SRC openbsd/warnx.c         )
list(APPEND SRC regex/regcomp.c         )
list(APPEND SRC regex/regerror.c        )
list(APPEND SRC regex/regexec.c         )
list(APPEND SRC regex/regfree.c         )
list(APPEND SRC vi/getc.c               )
list(APPEND SRC vi/v_at.c               )
list(APPEND SRC vi/v_ch.c               )
list(APPEND SRC vi/v_cmd.c              )
list(APPEND SRC vi/v_delete.c           )
list(APPEND SRC vi/v_ex.c               )
list(APPEND SRC vi/vi.c                 )
list(APPEND SRC vi/v_increment.c        )
list(APPEND SRC vi/v_init.c             )
list(APPEND SRC vi/v_itxt.c             )
list(APPEND SRC vi/v_left.c             )
list(APPEND SRC vi/v_mark.c             )
list(APPEND SRC vi/v_match.c            )
list(APPEND SRC vi/v_paragraph.c        )
list(APPEND SRC vi/v_put.c              )
list(APPEND SRC vi/v_redraw.c           )
list(APPEND SRC vi/v_replace.c          )
list(APPEND SRC vi/v_right.c            )
list(APPEND SRC vi/v_screen.c           )
list(APPEND SRC vi/v_scroll.c           )
list(APPEND SRC vi/v_search.c           )
list(APPEND SRC vi/v_section.c          )
list(APPEND SRC vi/v_sentence.c         )
list(APPEND SRC vi/vs_line.c            )
list(APPEND SRC vi/vs_msg.c             )
list(APPEND SRC vi/vs_refresh.c         )
list(APPEND SRC vi/vs_relative.c        )
list(APPEND SRC vi/vs_smap.c            )
list(APPEND SRC vi/vs_split.c           )
list(APPEND SRC vi/v_status.c           )
list(APPEND SRC vi/v_txt.c              )
list(APPEND SRC vi/v_ulcase.c           )
list(APPEND SRC vi/v_undo.c             )
list(APPEND SRC vi/v_util.c             )
list(APPEND SRC vi/v_word.c             )
list(APPEND SRC vi/v_xchar.c            )
list(APPEND SRC vi/v_yank.c             )
list(APPEND SRC vi/v_z.c                )
list(APPEND SRC vi/v_zexit.c            )

###############################################################################

set(AWK "awk") 
# set(PRINTF "printf")
# set(TEST "test")
# set(VERBOSE "@echo")

# set(DEBUG true)


# if(NOT DEBUG)
#     add_custom_command(
#         OUTPUT ex/ex_def.h
#        #  COMMAND ${VERBOSE} "\r\t${AWK}:\t%42s\n" "ex/ex.awk"
#        #  COMMAND ${AWK} -f "./ex/ex.awk" "./ex/ex_cmd.c" > "./ex/ex_def.h"
#         COMMAND ${TEST} -f "./ex/ex_def.h"
#         DEPENDS ex/ex.awk ex/ex_cmd.c
#         COMMENT "Generating ex/ex_def.h from ex/ex.awk and ex/ex_cmd.c"
#     )
# else()
#        execute_process(
#               COMMAND ${AWK} -f "./ex/ex.awk" "./ex/ex_cmd.c" > "./ex/ex_def.h"
#               WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#               RESULT_VARIABLE result
#        )

# #     add_custom_command(
# #         OUTPUT ex/ex_def.h
# #        #  COMMAND ${AWK} -f "./ex/ex.awk" "./ex/ex_cmd.c" > "./ex/ex_def.h"
# #         COMMAND ${TEST} -f "./ex/ex_def.h"
# #         DEPENDS ex/ex.awk ex/ex_cmd.c
# #         COMMENT "Generating ex/ex_def.h from ex/ex.awk and ex/ex_cmd.c"
# #     )
# endif()

# if(NOT DEBUG)
#     add_custom_command(
#         OUTPUT common/options_def.h
#         COMMAND ${VERBOSE} "\r\t${AWK}:\t%42s\n" "common/options.awk"
#         COMMAND ${RMF} "./common/options_def.h"
#         COMMAND ${AWK} -f "./common/options.awk" "./common/options_cmd.c" > "./common/options_def.h"
#         COMMAND ${TEST} -f "./common/options_def.h"
#         DEPENDS common/options.awk common/options_cmd.c
#         COMMENT "Generating common/options_def.h from common/options.awk and common/options_cmd.c"
#     )
# else()
# #     add_custom_command(
# #         OUTPUT common/options_def.h
# #         COMMAND ${RMF} "./common/options_def.h"
# #         COMMAND ${AWK} -f "./common/options.awk" "./common/options_cmd.c" > "./common/options_def.h"
# #         COMMAND ${TEST} -f "./common/options_def.h"
# #         DEPENDS common/options.awk common/options_cmd.c
# #         COMMENT "Generating common/options_def.h from common/options.awk and common/options_cmd.c"
# #     )
# endif()


execute_process(
       COMMAND ${AWK} -f "./ex/ex.awk" "./ex/ex_cmd.c" 
       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
       OUTPUT_FILE "./common/ex_def.h"
)

execute_process(
       COMMAND ${AWK} -f "./common/options.awk" "./common/options.c" 
       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
       OUTPUT_FILE "./common/options_def.h"
)


idf_component_register(SRCS  ${SRC} ${XSRC}
                       INCLUDE_DIRS cl include common regex openbsd mcurses/include 
                       REQUIRES vfs esp_driver_uart)
