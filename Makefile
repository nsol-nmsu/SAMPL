# Rebuild all four entities



all: company enforcer judge law


company:
	$(MAKE) -C Company/

enforcer:
	$(MAKE) -C Enforcer/

judge:
	$(MAKE) -C Judge/

law:
	$(MAKE) -C Law/


