import orientation, dashboard.resources.woezel_repo as woezel_repo, term, easydraw, system, ugfx, time, wifi, uos, json, sys, gc, woezel

system.serialWarning()

repo = woezel_repo.Repository()

orientation.default()

def showMessage(msg, error=False):
	term.header(True, "Installer")
	print(msg)

# Categories list

def show_categories():
	system.serialWarning()
	opt = []
	for category in repo.categories:
		opt.append(category["name"])
	opt.append("< Back to launcher")
	sel = term.menu("Installer - Categories", opt)
	if sel == len(repo.categories):
		system.launcher(True)
	show_category(sel)

# Category browsing

def show_category(i):
	system.serialWarning()
	global category
	slug = repo.categories[i]["slug"]
	name = repo.categories[i]["name"]
	showMessage("Loading "+slug+"...")
	try:
		try:
			category = repo.getCategory(slug)
		except:
			showMessage("Failed to open category "+slug+"!", True)
			time.sleep(1)
			show_categories()
		gc.collect()
		opt = []
		for package in category:
			opt.append("%s rev. %s" % (package["name"], package["revision"]))
		opt.append("< Back to categories")
		sel = term.menu("Installer - "+name, opt)
		if sel == len(category):
			show_categories()
		install_app(sel)
	except BaseException as e:
		sys.print_exception(e)
		showMessage(e, True)
		time.sleep(1)
		show_categories()

# Install application

def install_app(i):
	system.serialWarning()
	global category
	slug = category[i]["slug"]
	category = None
	gc.collect()
	if not wifi.status():
		wifi.connect()
		wifi.wait()
		if not wifi.status():
			showMessage("Unable to connect to WiFi.")
			time.sleep(2)
			show_category()
	showMessage("Installing "+slug+"...")
	try:
		woezel.install(slug)
	except woezel.LatestInstalledError:
		showMessage("Latest version is already installed.")
		time.sleep(2)
		show_category()
	except:
		showMessage("Failed to install "+slug+"!")
		time.sleep(2)
		show_category()
	showMessage(slug+" has been installed!")
	show_category()

#Main application

showMessage("Loading categories...")
if not repo.load():
	if not repo.update():
		if repo.lastUpdate==0:
			showMessage("Failed to load repository. Returning to launcher...")
			system.launcher()

show_categories()
