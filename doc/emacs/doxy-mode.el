;; fold and unfold utilities for hiding doxygen comment blocks. one
;; day this could grow up to become a full-fledged minor mode
(provide 'doxy-mode)

(global-set-key [(control ?c) (control ?f)] 'doxy-fold-all)
(global-set-key [(control ?c) (control ?g)] 'doxy-unfold-all)

;; --

(defun doxy-fold-all ()
  "loop through and fold all the doxy-fold- overlays"
  (interactive "")
  (save-excursion
    (doxy-fold-scan)
    (let ((n 0))
      (while (< n (length doxy-folded-subs-list))
        (doxy-fold-overlay-props (nth n doxy-folded-subs-list))
        (setq n (+ n 1))))))


(defun doxy-unfold-all ()
  "delete all overlays in the doxy-folded-subs-list"
  (interactive "")
  (save-excursion
    (let ((overlay (car doxy-folded-subs-list)))
      (while overlay
        (delete-overlay overlay)
        (setq doxy-folded-subs-list (cdr doxy-folded-subs-list))
        (setq overlay (car doxy-folded-subs-list))))))

;; --

(setq-default doxy-folded-subs-list (list))
(setq line-move-ignore-invisible t)
(defun doxy-fold-scan ()
  "start from top of buffer and make overlays for all doxygen comment blocks"
  (interactive "")
  (save-excursion
    (doxy-unfold-all)
    (goto-char (point-min))
    (doxy-fold-scan-c++-style-blocks)
    (doxy-fold-scan-c-style-multilines)
    (doxy-fold-scan-grouping-lines)))

(defun doxy-fold-scan-c++-style-blocks ()
  (save-excursion
    (let ((fold-start nil)
          (fold-stop nil)
          (overlay nil))
      (while
          (re-search-forward "/\\*[\\*\\!]" (point-max) t)
        (beginning-of-line)
        (setq fold-start (point))
        (re-search-forward "\\*/" (point-max) t)
        (forward-line)
        (setq fold-stop (point))
        (setq doxy-folded-subs-list
              (cons (make-overlay fold-start fold-stop) doxy-folded-subs-list))
        (setq overlay (car doxy-folded-subs-list))
        (overlay-put overlay 'doxy-folded-before-string "@")
        (overlay-put overlay 'after-string " ")))))

(defun doxy-fold-scan-c-style-multilines ()
  (save-excursion
    (let ((fold-start nil)
          (fold-stop nil)
          (overlay nil))
      (while
          (re-search-forward "//[/\\!]" (point-max) t)
        (beginning-of-line)
        (setq fold-start (point))
        (forward-line 1)
        (while
            (re-search-forward "//[/\\!]" (+ (point) 12) t)
          (forward-line 1))
        (re-search-forward "\\w" (point-max) t)(backward-char 1)
        (setq fold-stop (point))
        (setq doxy-folded-subs-list
              (cons (make-overlay fold-start fold-stop) doxy-folded-subs-list))
        (setq overlay (car doxy-folded-subs-list))
        (overlay-put overlay 'doxy-folded-before-string "@")
        (overlay-put overlay 'after-string " ")))))

(defun doxy-fold-scan-grouping-lines ()
  (save-excursion
    (let ((fold-start nil)
          (fold-stop nil)
          (overlay nil))
      (while
          (re-search-forward "//@[{}]" (point-max) t)
        (beginning-of-line)
        (setq fold-start (point))
        (forward-line 1)
        (setq fold-stop (point))
        (setq doxy-folded-subs-list
              (cons (make-overlay fold-start fold-stop) doxy-folded-subs-list))
        (setq overlay (car doxy-folded-subs-list))
        (overlay-put overlay 'doxy-folded-before-string "")
        (overlay-put overlay 'after-string "")))))


(defun doxy-fold-overlay-props (overlay)
  (overlay-put overlay 'doxy-folded-sub-overlay t)
;  (overlay-put overlay 'intangible t)
  (overlay-put overlay 'invisible t)
  (overlay-put overlay 'before-string
               (overlay-get overlay 'doxy-folded-before-string))
  ;(overlay-put overlay 'after-string " ")
  )

(defun doxy-unfold-overlay-props (overlay)
  (overlay-put overlay 'intangible nil)
  (overlay-put overlay 'invisible nil)
  (overlay-put overlay 'before-string "")
  (overlay-put overlay 'after-string ""))
