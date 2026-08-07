#set default browser
xdg-settings set default-web-browser app.zen_browser.zen.desktop
xdg-settings set default-url-scheme-handler http app.zen_browser.zen.desktop
xdg-settings set default-url-scheme-handler https app.zen_browser.zen.desktop
xdg-mime default app.zen_browser.zen.desktop text/html

#set default text editor
xdg-mime default nvim.desktop text/plain

#set default image viewer
xdg-mime default org.kde.gwenview.desktop image/png
xdg-mime default org.kde.gwenview.desktop image/jpeg
xdg-mime default org.kde.gwenview.desktop image/jpg
xdg-mime default org.kde.gwenview.desktop image/webp
xdg-mime default org.kde.gwenview.desktop image/gif
xdg-mime default org.kde.gwenview.desktop image/svg+xml

#set default pdf viewer
xdg-mime default org.gnome.Evince.desktop application/pdf

#check default browser
xdg-mime query default x-scheme-handler/https
