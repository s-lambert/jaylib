(use ../build/jaylib)

(init-window 800 600 "Test Game")
(set-target-fps 60)

(var toggle_active (gui-boolean false))
(var toggle_group_active (gui-integer 1))
(var toggle_slider_active (gui-integer 1))
(var check_active (gui-boolean false))
(var combo_active (gui-integer 1))
(var dropdown_active (gui-integer 1))
(var dropdown_open false)
(var loading_spinner (gui-integer 0))
(var editing_number false)
(var number_input (gui-integer 5))
(var editing_text false)
(var text_input @"text")
(var slider_value (gui-float 1.5))
(var selected_cell @[-1 -1])
(var list_scroll (gui-integer 0))
(var selected_list (gui-integer -1))
(var focus_list (gui-integer -1)) 
(var show_password (gui-boolean false))
(var password_input @"")

(def nine-patch (load-image-1 "test/nine-patch.png"))
(def nine-patch-texture (load-texture-from-image nine-patch))

(while (not (window-should-close))
  (begin-drawing)

  (clear-background :ray-white)

  # Lock the UI while the dropdown is open
  (if dropdown_open (gui-lock))

  (gui-label [0 0 75 25] "LABEL")
  (if (gui-button [0 25 75 25] "BUTTON")
    (print "clicked button"))
  (if (gui-label-button [0 50 75 25] "LABEL_BUTTON")
    (print "clicked label button"))
  (gui-toggle [0 75 75 25] "TOGGLE" toggle_active)
  (gui-toggle-group [0 100 75 25] "1;2;3" toggle_group_active)
  (gui-toggle-slider [0 125 75 25] "1;2;3" toggle_slider_active)
  (gui-check-box [0 150 25 25] "CHECKBOX" check_active)
  (gui-combo-box [0 175 75 25] "1;2;3" combo_active)
  (if (gui-value-box [0 225 75 25] "NUMBER" number_input 0 10 editing_number)
    (set editing_number (not editing_number)))
  (gui-spinner [125 0 75 25] "SPINNER" loading_spinner 0 100 false)
  (if (gui-text-box [0 250 75 25] text_input 10 editing_text)
    (set editing_text (not editing_text)))
  (gui-slider [25 275 75 25] "0" "10" slider_value 0 10)
  (gui-slider-bar [25 300 75 25] "0" "10" slider_value 0 10)
  (gui-progress-bar [25 325 75 25] "0" "10" slider_value 0 10)
  (gui-status-bar [0 575 800 25] "STATUS")
  (gui-dummy-rec [0 350 50 25] "BOX")
  (gui-grid [0 375 200 200] "GRID" (number_input :value) (math/round (slider_value :value)) selected_cell)
  (gui-list-view [200 0 50 100] "ONE;TWO;THREE;FOUR;FIVE;SIX" list_scroll selected_list)
  (gui-list-view-ex [250 0 50 100] '("ONE" "TWO" "THREE" "FOUR" "FIVE" "SIX") 6 list_scroll selected_list focus_list)
  (let [button-clicked (gui-message-box [300 0 200 100] "TITLE" "MESSAGE" "OK;CANCEL")]
    (cond
      (= button-clicked 0) (print "clicked close button")
      (= button-clicked 1) (print "clicked OK")
      (= button-clicked 2) (print "clicked cancel")))
  (let [button-clicked (gui-text-input-box [500 0 200 125] "LOGIN" "PASSWORD?" "OK;CANCEL" password_input 10 nil)]
    (cond
      (= button-clicked 0) (print "clicked close button")
      (= button-clicked 1) (print "clicked OK")
      (= button-clicked 2) (print "clicked cancel")))

  (draw-texture-n-patch nine-patch-texture [[0 0 96 96] 32 32 32 32 0] [300 300 200 200] [0 0] 0 :white)

  # Unlock and then render the dropdown so it's still interactable
  (gui-unlock)

  (if (gui-dropdown-box [0 200 75 25] "1;2;3" dropdown_active dropdown_open)
    (set dropdown_open (not dropdown_open)))

  (end-drawing))

(close-window)
