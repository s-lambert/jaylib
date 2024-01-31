(use ../build/jaylib)

(init-window 800 600 "Test Game")
(set-target-fps 60)

(var toggle_active (gui-active false))
(var check_active (gui-active false))

(while (not (window-should-close))
  (begin-drawing)
  (gui-label [0 0 75 25] "LABEL")
  (if (gui-button [0 25 75 25] "BUTTON")
    (print "clicked button"))
  (if (gui-label-button [0 50 75 25] "LABEL_BUTTON")
    (print "clicked label button"))
  (gui-toggle [0 75 75 25] "TOGGLE" toggle_active)
  (gui-checkbox [0 100 75 25] "CHECKBOX" check_active)
  (end-drawing))

(close-window)
