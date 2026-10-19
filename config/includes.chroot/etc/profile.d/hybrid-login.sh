# Start the Hybrid manager on interactive login shells.
case "$-" in
  *i*)
    if [ -x /usr/local/sbin/hybrid ] && [ -z "${HYBRID_SHELL_STARTED:-}" ]; then
      export HYBRID_SHELL_STARTED=1
      /usr/local/sbin/hybrid
    fi
    ;;
esac
