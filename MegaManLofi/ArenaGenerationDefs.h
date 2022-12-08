#pragma once

#include <string>

namespace MegaManLofi
{
   class ArenaGenerationDefs
   {
   public:
      // 's': solid, blocks in all directions
      // 'p': platform, only blocks downward
      // 'x': spikes only blocks upward
      static std::string GetArenaTilesString()
      {
         std::string tiles =
            "                                                                                                                                                                                                                                                                                                                                         sssssssssssssssssssssssssssssss" \
            "                                                                                                                                                                                                                                                                                                                                         s                              " \
            "                                                                                                                                                                                                                                                                                                                                         s                              " \
            "                                                                                                                                                                                                                                                                                                                                         s                              " \
            "                                                                                                                                                                                                                                                                                                                                         s                              " \
            "                                                                                                                                                                                                                                                                                                                                         s              ssssssssssssssss" \
            "                                                                                                                                                                                                                                                                                                                                         s                             s" \
            "                                                                                                                                                                                                                                                                                                                                         s                             s" \
            "                                                                                                                                                                                                                                                                                                                                         s                             s" \
            "ssssssssssssssssssssssssssssss                                                                                                                                                                                                                                                                                                           s                             s" \
            "s                                                                                                                                                                                                                                                                                                                                        sssssssssssssssssss           s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                     ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" \
            "s                                          sssssssssssssss                                                                                                                                                                                                                                                                                                             s" \
            "s                                                                                                                                                                                                                                                        ssssssssssssssssssssss                                                                                        s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                            ssssssssssssssssssssssssssssssssssssssssss                                                                                                                                                                                s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s    sssssssssssssssssssssssssssss                                                          ssssssssssssssssssssssssssssssssssss                                                                                                                                                                                                                                       s" \
            "s                                                                                                                                                                                                      ssssssssssssssssssssssssssssssssssssssssss                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                            sssssssssssssssss              ssssssssssssss                                                                                                                                                                                                                                                                                             s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                           ssssssssssssxxxxxxxxxxxxxxxxxssssssssssssssss                                                                                                                                                                              s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                 pppppppppppssssssssssss                                                                                                                                                                                                                                                              s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                               ssss                                                   s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                               sssssssssssssssssssssssssss                                                                                                                                                                                                            s" \
            "s                                                           sssssxxxxxsssssssss                                                                                                                                                                                                                                                                                        s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "spppppppppppppssssssssssssssssssssss                                                                                                                                                                                                                                                                                                                                   s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                  ssss                                s" \
            "s                                                                                  pppppppppppppppppppppppppppppppppp                                                                                                                                                                                                                                                  s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "sssssssss                                                                                                                                                                                                                                                                                                                                                              s" \
            "s                                                                                                                                                                                                                                                                                                        ssss                                                          s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "s                                                                                                                                                                                                                                                                                                                                                                      s" \
            "ssssssssssssssssssssssssssssssssssssssssss                                  sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss                   sssssssssssssssssssssssssssssssssssssssssss                                                                                            s";

         return tiles;
      }
   };
}
