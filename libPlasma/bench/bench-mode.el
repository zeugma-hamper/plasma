(defconst bench--bstart-rx
  "\\<\\(start_\\(?:clean_\\)?benchmark\\)\\>\\(.+\\)?")

(defconst bench--with-form-rx
  (regexp-opt '("with_pool" "with_new_pool" "end_pool"
                "with_existing_pool" "with_existing_pools") 'words))

(defconst bench--poolname-rx
  "\\<\\(?:with_\\(?:new_\\|existing_\\)?pool\\) +\\([^ \n]+\\)")

(defconst bench--poolnames-rx "\\<with_existing_pools\\>\\(.+\\)")

(defconst bench--spoolname-rx "\\<[rsu]/\\([^ ]+\\)\\>")

(defconst bench--proc-forms '("butter" "bread" "sandwich" "jam" "braid"))

(defconst bench--proc-forms-rx
  (format "\\<\\(%s\\(?:_p\\)?\\)\\>" (regexp-opt bench--proc-forms)))

(defconst bench--proc-read-modes
  '("forward" "oldest"  "newest" "backward" "tail" "random"))

(defconst bench--proc-read-modes-rx
  (format "\\<\\(%s\\)\\(/\\|\\>\\)" (regexp-opt bench--proc-read-modes)))

(defconst bench--batches-rx
  (format "%s +\\([0-9]+\\) +\\([0-9]+\\)?" bench--proc-forms-rx))


(defconst bench--sspec-rx "\\<\\([usr]\\)\\(/[^ ]*\\|\\>\\)")

(setq bench--keywords
      `((,bench--bstart-rx (1 'font-lock-warning-face)
                           (2 'font-lock-string-face nil t))
        ("\\<end_benchmark\\>" . 'font-lock-warning-face)
        (,bench--with-form-rx . 'font-lock-type-face)
        (,bench--poolname-rx 1 'font-lock-string-face)
        (,bench--poolnames-rx 1 'font-lock-string-face)
        (,bench--spoolname-rx 1 'font-lock-string-face)
        (,bench--proc-forms-rx 1 'font-lock-function-name-face)
        (,bench--proc-read-modes-rx 1 'font-lock-keyword-face)
        (,bench--batches-rx (2 'font-lock-constant-face)
                            (3 'font-lock-constant-face nil t))
        (,bench--sspec-rx 1 'font-lock-warning-face)))

(define-derived-mode bench-mode sh-mode "Bench"
  "Benchmark scripts mode"
  (font-lock-add-keywords nil bench--keywords)
  (sh-set-shell "/usr/bin/bash" t nil))


(add-to-list 'auto-mode-alist '("\\.bench$" . bench-mode))

(provide 'bench-mode)
