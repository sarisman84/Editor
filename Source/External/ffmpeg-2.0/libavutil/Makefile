include $(SUBDIR)../config.mak

NAME = avutil

HEADERS = adler32.h                                                     \
          aes.h                                                         \
          attributes.h                                                  \
          audio_fifo.h                                                  \
          audioconvert.h                                                \
          avassert.h                                                    \
          avstring.h                                                    \
          avutil.h                                                      \
          base64.h                                                      \
          blowfish.h                                                    \
          bprint.h                                                      \
          bswap.h                                                       \
          buffer.h                                                      \
          channel_layout.h                                              \
          common.h                                                      \
          cpu.h                                                         \
          crc.h                                                         \
          error.h                                                       \
          eval.h                                                        \
          fifo.h                                                        \
          file.h                                                        \
          frame.h                                                       \
          hmac.h                                                        \
          imgutils.h                                                    \
          intfloat.h                                                    \
          intfloat_readwrite.h                                          \
          intreadwrite.h                                                \
          lfg.h                                                         \
          log.h                                                         \
          mathematics.h                                                 \
          md5.h                                                         \
          mem.h                                                         \
          murmur3.h                                                     \
          dict.h                                                        \
          old_pix_fmts.h                                                \
          opt.h                                                         \
          parseutils.h                                                  \
          pixdesc.h                                                     \
          pixfmt.h                                                      \
          random_seed.h                                                 \
          rational.h                                                    \
          ripemd.h                                                      \
          samplefmt.h                                                   \
          sha.h                                                         \
          sha512.h                                                      \
          time.h                                                        \
          timecode.h                                                    \
          timestamp.h                                                   \
          version.h                                                     \
          xtea.h                                                        \

HEADERS-$(CONFIG_LZO)                   += lzo.h

HEADERS-$(CONFIG_OPENCL)                += opencl.h

ARCH_HEADERS = bswap.h                                                  \
               intmath.h                                                \
               intreadwrite.h                                           \
               timer.h                                                  \

BUILT_HEADERS = avconfig.h

OBJS = adler32.o                                                        \
       aes.o                                                            \
       atomic.o                                                         \
       audio_fifo.o                                                     \
       avstring.o                                                       \
       base64.o                                                         \
       blowfish.o                                                       \
       bprint.o                                                         \
       buffer.o                                                         \
       channel_layout.o                                                 \
       cpu.o                                                            \
       crc.o                                                            \
       des.o                                                            \
       error.o                                                          \
       eval.o                                                           \
       fifo.o                                                           \
       file.o                                                           \
       float_dsp.o                                                      \
       frame.o                                                          \
       hash.o                                                           \
       hmac.o                                                           \
       imgutils.o                                                       \
       intfloat_readwrite.o                                             \
       intmath.o                                                        \
       lfg.o                                                            \
       lls.o                                                            \
       log.o                                                            \
       log2_tab.o                                                       \
       mathematics.o                                                    \
       md5.o                                                            \
       mem.o                                                            \
       murmur3.o                                                        \
       dict.o                                                           \
       opt.o                                                            \
       parseutils.o                                                     \
       pixdesc.o                                                        \
       random_seed.o                                                    \
       rational.o                                                       \
       rc4.o                                                            \
       ripemd.o                                                         \
       samplefmt.o                                                      \
       sha.o                                                            \
       sha512.o                                                         \
       time.o                                                           \
       timecode.o                                                       \
       tree.o                                                           \
       utils.o                                                          \
       xga_font_data.o                                                  \
       xtea.o                                                           \

OBJS-$(CONFIG_LZO)                      += lzo.o
OBJS-$(CONFIG_OPENCL)                   += opencl.o opencl_internal.o

OBJS += $(COMPAT_OBJS:%=../compat/%)

SKIPHEADERS          = old_pix_fmts.h

SKIPHEADERS-$(HAVE_ATOMICS_GCC)        += atomic_gcc.h
SKIPHEADERS-$(HAVE_ATOMICS_SUNCC)      += atomic_suncc.h
SKIPHEADERS-$(HAVE_ATOMICS_WIN32)      += atomic_win32.h
SKIPHEADERS-$(CONFIG_OPENCL)           += opencl.h

TESTPROGS = adler32                                                     \
            aes                                                         \
            atomic                                                      \
            avstring                                                    \
            base64                                                      \
            blowfish                                                    \
            bprint                                                      \
            cpu                                                         \
            crc                                                         \
            des                                                         \
            error                                                       \
            eval                                                        \
            file                                                        \
            fifo                                                        \
            hmac                                                        \
            lfg                                                         \
            lls                                                         \
            md5                                                         \
            murmur3                                                     \
            opt                                                         \
            pca                                                         \
            parseutils                                                  \
            random_seed                                                 \
            rational                                                    \
            ripemd                                                      \
            sha                                                         \
            sha512                                                      \
            tree                                                        \
            xtea                                                        \

TESTPROGS-$(HAVE_LZO1X_999_COMPRESS) += lzo

TOOLS = ffhash ffeval ffescape

$(SUBDIR)lzo-test$(EXESUF): ELIBS = -llzo2
